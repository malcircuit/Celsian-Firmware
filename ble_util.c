/*
 * ble.c
 *
 *  Created on: Aug 22, 2016
 *      Author: matt
 */

#include "ble_util.h"

static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;   /**< Handle of the current connection. */
static uint16_t celsian_srv_handle;

static ble_gatts_char_handles_t mpl_temp_handle;
static ble_gatts_char_handles_t sht_temp_handle;
static ble_gatts_char_handles_t rh_handle;
static ble_gatts_char_handles_t pres_handle;
static ble_gatts_char_handles_t uva_handle;
static ble_gatts_char_handles_t uvb_handle;
static ble_gatts_char_handles_t uvd_handle;
static ble_gatts_char_handles_t uvcomp1_handle;
static ble_gatts_char_handles_t uvcomp2_handle;

static uint8_t *mpl_temp, *sht_temp, *rh, *pres, *uva, *uvb, *uvd, *uvcomp1, *uvcomp2;

static const ble_uuid128_t celsian_base_uuid = CELSIAN_BASE_UUID;
static ble_uuid_t sht_temp_uuid = { .uuid = CELSIAN_SHT_TEMP_CHAR_UUID, 	.type = CELSIAN_SERVICE_UUID_TYPE };
static ble_uuid_t mpl_temp_uuid = { .uuid = CELSIAN_MPL_TEMP_CHAR_UUID, 	.type = CELSIAN_SERVICE_UUID_TYPE };
static ble_uuid_t rh_uuid 		= { .uuid = CELSIAN_RH_CHAR_UUID, 			.type = CELSIAN_SERVICE_UUID_TYPE };
static ble_uuid_t pres_uuid 	= { .uuid = CELSIAN_PRES_CHAR_UUID, 		.type = CELSIAN_SERVICE_UUID_TYPE };
static ble_uuid_t uva_uuid 		= { .uuid = CELSIAN_UVA_CHAR_UUID, 			.type = CELSIAN_SERVICE_UUID_TYPE };
static ble_uuid_t uvb_uuid 		= { .uuid = CELSIAN_UVB_CHAR_UUID, 			.type = CELSIAN_SERVICE_UUID_TYPE };
static ble_uuid_t uvd_uuid 		= { .uuid = CELSIAN_UVD_CHAR_UUID, 			.type = CELSIAN_SERVICE_UUID_TYPE };
static ble_uuid_t uvcomp1_uuid 	= { .uuid = CELSIAN_UVCOMP1_CHAR_UUID, 		.type = CELSIAN_SERVICE_UUID_TYPE };
static ble_uuid_t uvcomp2_uuid 	= { .uuid = CELSIAN_UVCOMP2_CHAR_UUID, 		.type = CELSIAN_SERVICE_UUID_TYPE };

static ble_uuid_t celsian_srv_uuid = {
		.type = CELSIAN_SERVICE_UUID_TYPE,
		.uuid = CELSIAN_SERVICE_UUID
};

static ble_uuid_t m_adv_uuids[] = {{.uuid = CELSIAN_SERVICE_UUID, .type = CELSIAN_SERVICE_UUID_TYPE	}};

static ble_gatts_char_pf_t celsian_temp_format =
{
		.format = BLE_GATT_CPF_FORMAT_FLOAT64,				// double
		.exponent = 0,										// Start at 0
		.unit = UNITS_DEGREES_C,							// Units of degrees Celsius
		.name_space = BLE_GATT_CPF_NAMESPACE_BTSIG,			// Normal namespace
		.desc = BLE_GATT_CPF_NAMESPACE_DESCRIPTION_UNKNOWN	// No description for this format
};

static ble_gatts_char_pf_t celsian_rh_format =
{
		.format = BLE_GATT_CPF_FORMAT_FLOAT64,				// double
		.exponent = 0,										// Start at 0
		.unit = UNITS_PERCENT,								// Units of percentage
		.name_space = BLE_GATT_CPF_NAMESPACE_BTSIG,			// Normal namespace
		.desc = BLE_GATT_CPF_NAMESPACE_DESCRIPTION_UNKNOWN	// No description for this format
};

static ble_gatts_char_pf_t celsian_pres_format =
{
		.format = BLE_GATT_CPF_FORMAT_FLOAT64,				// double
		.exponent = 0,										// Start at 0
		.unit = UNITS_PASCALS,								// Units of pascals
		.name_space = BLE_GATT_CPF_NAMESPACE_BTSIG,			// Normal namespace
		.desc = BLE_GATT_CPF_NAMESPACE_DESCRIPTION_UNKNOWN	// No description for this format
};

static ble_gatts_char_pf_t celsian_uv_format =
{
		.format = BLE_GATT_CPF_FORMAT_UINT16,				// Unsigned 16-bit integer
		.exponent = 0,										// Start at 0
		.unit = UNITLESS,									// Value has no physical significance
		.name_space = BLE_GATT_CPF_NAMESPACE_BTSIG,			// Normal namespace
		.desc = BLE_GATT_CPF_NAMESPACE_DESCRIPTION_UNKNOWN	// No description for this format
};

static ble_callbacks_t *callbacks;

/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num   Line number of the failing ASSERT call.
 * @param[in] file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *) DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

static void atmo_chars_add()
{
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_t    attr_char_value;
	ble_gatts_attr_md_t attr_md;

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.read			 = true;
	char_md.char_props.broadcast     = true;
	char_md.p_char_user_desc         = NULL;
	char_md.char_user_desc_max_size  = 0;
	char_md.char_user_desc_size		 = 0;
	char_md.p_char_pf                = &celsian_temp_format;
	char_md.p_user_desc_md           = NULL;
	char_md.p_cccd_md           	 = NULL;
	char_md.p_sccd_md                = NULL;

	memset(&attr_md, 0, sizeof(attr_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

	attr_md.vloc    = BLE_GATTS_VLOC_USER;	// R/W characteristic value directly from a variable in RAM
	attr_md.rd_auth = false;
	attr_md.wr_auth = false;
	attr_md.vlen    = false;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid    = &mpl_temp_uuid;
	attr_char_value.p_attr_md = &attr_md;
	attr_char_value.init_len  = 8;
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = 8;
	attr_char_value.p_value	  = mpl_temp;

	APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(celsian_srv_handle, &char_md, &attr_char_value, &mpl_temp_handle));

	attr_char_value.p_uuid    = &sht_temp_uuid;
	attr_char_value.p_value	  = sht_temp;

	APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(celsian_srv_handle, &char_md, &attr_char_value, &sht_temp_handle));

	char_md.p_char_pf         = &celsian_rh_format;
	attr_char_value.p_uuid    = &rh_uuid;
	attr_char_value.p_value	  = rh;

	APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(celsian_srv_handle, &char_md, &attr_char_value, &rh_handle));

	char_md.p_char_pf         = &celsian_pres_format;
	attr_char_value.p_uuid    = &pres_uuid;
	attr_char_value.p_value	  = pres;

	APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(celsian_srv_handle, &char_md, &attr_char_value, &pres_handle));
}

static void uv_chars_add()
{
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_t    attr_char_value;
	ble_gatts_attr_md_t attr_md;

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.read			 = true;
	char_md.char_props.broadcast     = true;
	char_md.p_char_user_desc         = NULL;
	char_md.char_user_desc_max_size  = 0;
	char_md.char_user_desc_size		 = 0;
	char_md.p_char_pf                = &celsian_uv_format;
	char_md.p_user_desc_md           = NULL;
	char_md.p_cccd_md           	 = NULL;
	char_md.p_sccd_md                = NULL;

	memset(&attr_md, 0, sizeof(attr_md));

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

	attr_md.vloc    = BLE_GATTS_VLOC_USER;	// R/W characteristic value directly from a variable in RAM
	attr_md.rd_auth = false;
	attr_md.wr_auth = false;
	attr_md.vlen    = false;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_attr_md = &attr_md;
	attr_char_value.init_len  = 2;
	attr_char_value.init_offs = 0;
	attr_char_value.max_len   = 2;

	attr_char_value.p_uuid    = &uva_uuid;
	attr_char_value.p_value	  = uva;

	APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(celsian_srv_handle, &char_md, &attr_char_value, &uva_handle));

	attr_char_value.p_uuid    = &uvb_uuid;
	attr_char_value.p_value	  = uvb;

	APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(celsian_srv_handle, &char_md, &attr_char_value, &uvb_handle));

	attr_char_value.p_uuid    = &uvd_uuid;
	attr_char_value.p_value	  = uvd;

	APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(celsian_srv_handle, &char_md, &attr_char_value, &uvd_handle));

	attr_char_value.p_uuid    = &uvcomp1_uuid;
	attr_char_value.p_value	  = uvcomp1;

	APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(celsian_srv_handle, &char_md, &attr_char_value, &uvcomp1_handle));

	attr_char_value.p_uuid    = &uvcomp2_uuid;
	attr_char_value.p_value	  = uvcomp2;

	APP_ERROR_CHECK(sd_ble_gatts_characteristic_add(celsian_srv_handle, &char_md, &attr_char_value, &uvcomp2_handle));
}

/**@brief Function for initializing services that will be used by the application.
 */
static void services_init(void)
{
	APP_ERROR_CHECK(sd_ble_uuid_vs_add(&celsian_base_uuid, &(celsian_srv_uuid.type)));
	APP_ERROR_CHECK(sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &celsian_srv_uuid, &celsian_srv_handle));

	atmo_chars_add();
	uv_chars_add();
}


/**@brief Function for handling the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module which
 *          are passed to the application.
 *          @note All this function does is to disconnect. This could have been done by simply
 *                setting the disconnect_on_fail config parameter, but instead we use the event
 *                handler mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for starting timers.
*/
//static void application_timers_start(void)
//{
//    /* YOUR_JOB: Start your timers. below is an example of how to start a timer.
//    uint32_t err_code;
//    err_code = app_timer_start(m_app_timer_id, TIMER_INTERVAL, NULL);
//    APP_ERROR_CHECK(err_code); */
//
//}


/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
        	callbacks->on_adv_start();
            break;
        case BLE_ADV_EVT_IDLE:
        	callbacks->on_adv_idle();
            break;
        default:
            break;
    }
}


/**@brief Function for handling the Application's BLE Stack events.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
	switch (p_ble_evt->header.evt_id)
	{
	case BLE_GAP_EVT_CONNECTED:
		m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
		callbacks->on_connect();
		break;

	case BLE_GAP_EVT_DISCONNECTED:
		m_conn_handle = BLE_CONN_HANDLE_INVALID;
		callbacks->on_disconnect();
		break;

	default:
		// No implementation needed.
		break;
	}
}


/**@brief Function for dispatching a BLE stack event to all modules with a BLE stack event handler.
 *
 * @details This function is called from the BLE Stack event interrupt handler after a BLE stack
 *          event has been received.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_params_on_ble_evt(p_ble_evt);
    on_ble_evt(p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);
}


/**@brief Function for dispatching a system event to interested modules.
 *
 * @details This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * @param[in] sys_evt  System stack event.
 */
static void sys_evt_dispatch(uint32_t sys_evt)
{
    pstorage_sys_event_handler(sys_evt);
    ble_advertising_on_sys_evt(sys_evt);
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    uint32_t err_code;

//    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(NULL, NULL);

    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);

    //Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Advertising functionality.
 */
static void advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t srdata;

    // Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type				= BLE_ADVDATA_FULL_NAME;
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    ble_adv_modes_config_t options 	= {0};
    options.ble_adv_fast_enabled  	= BLE_ADV_FAST_ENABLED;
    options.ble_adv_fast_interval 	= APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  	= APP_ADV_TIMEOUT_IN_SECONDS;

    memset(&srdata, 0, sizeof(srdata));

    srdata.uuids_complete.uuid_cnt 	= sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    srdata.uuids_complete.p_uuids 	= m_adv_uuids;

    err_code = ble_advertising_init(&advdata, &srdata, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);
}


void ble_init(ble_callbacks_t *event_callbacks, ble_char_value_handles_t values)
{
	mpl_temp = values.p_mpl_temp;
	sht_temp = values.p_sht_temp;
	rh = values.p_rh;
	pres = values.p_pres;
	uva = values.p_uva;
	uvb = values.p_uvb;
	uvd = values.p_uvd;
	uvcomp1 = values.p_uvcomp1;
	uvcomp2 = values.p_uvcomp2;

	ble_stack_init();
	gap_params_init();
	services_init();
	advertising_init();
	conn_params_init();

	callbacks = event_callbacks;
}

void ble_start_adv()
{
	APP_ERROR_CHECK(ble_advertising_start(BLE_ADV_MODE_FAST));
}

void ble_disconnect()
{
	sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
}
