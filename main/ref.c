#if 0
/*
   ESP32-C3 Matter 센서 예제에서 Basic Information 설정
   
   사용법:
   1. 이 코드를 센서 예제의 app_main.cpp에서 참조
   2. esp_matter::start() 호출 전에 set_basic_information() 호출
   3. IDF v5.4.2, Matter v1.4 호환
*/

#include <esp_err.h>
#include <esp_log.h>
#include <nvs_flash.h>

#include <esp_matter.h>
#include <esp_matter_console.h>
#include <esp_matter_ota.h>
#include <esp_matter_endpoint.h>

#include <common_macros.h>
#include <app_priv.h>
#include <app_reset.h>

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include <platform/ESP32/OpenthreadLauncher.h>
#endif

static const char *TAG = "app_main";
uint16_t sensor_endpoint_id = 0;

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;
using namespace chip::app::Clusters;

// 센서 예제에서 Basic Information 설정 함수
static esp_err_t set_basic_information_for_sensor(node_t *node) 
{
    ESP_LOGI(TAG, "Setting Basic Information for sensor device");

    // Root 엔드포인트 (Endpoint 0) 가져오기
    endpoint_t *root_endpoint = endpoint::get(node, 0);
    if (!root_endpoint) {
        ESP_LOGE(TAG, "Root endpoint not found");
        return ESP_FAIL;
    }

    // Basic Information 클러스터 가져오기
    cluster_t *basic_info_cluster = cluster::get(root_endpoint, BasicInformation::Id);
    if (!basic_info_cluster) {
        ESP_LOGE(TAG, "Basic Information cluster not found on root endpoint");
        return ESP_FAIL;
    }

    // Basic Information 속성 설정
    esp_err_t err = ESP_OK;

    // 1. Vendor ID 설정 (0xFFF1 = Test VID)
    attribute_t *vendor_id_attr = attribute::get(basic_info_cluster, BasicInformation::Attributes::VendorID::Id);
    if (vendor_id_attr) {
        esp_matter_attr_val_t vendor_id_val = esp_matter_uint16(0xFFF1);
        err = attribute::set_val(vendor_id_attr, &vendor_id_val);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "✓ Vendor ID set to: 0xFFF1");
        }
    }

    // 2. Product ID 설정 (센서용 Product ID)
    attribute_t *product_id_attr = attribute::get(basic_info_cluster, BasicInformation::Attributes::ProductID::Id);
    if (product_id_attr) {
        esp_matter_attr_val_t product_id_val = esp_matter_uint16(0x8002); // 센서용 Product ID
        err = attribute::set_val(product_id_attr, &product_id_val);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "✓ Product ID set to: 0x8002");
        }
    }

    // 3. Vendor Name 설정
    attribute_t *vendor_name_attr = attribute::get(basic_info_cluster, BasicInformation::Attributes::VendorName::Id);
    if (vendor_name_attr) {
        const char* vendor_name = "Espressif";
        esp_matter_attr_val_t vendor_name_val = esp_matter_char_str(vendor_name, strlen(vendor_name));
        err = attribute::set_val(vendor_name_attr, &vendor_name_val);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "✓ Vendor Name set to: %s", vendor_name);
        }
    }

    // 4. Product Name 설정
    attribute_t *product_name_attr = attribute::get(basic_info_cluster, BasicInformation::Attributes::ProductName::Id);
    if (product_name_attr) {
        const char* product_name = "ESP32-C3 Matter Sensor";
        esp_matter_attr_val_t product_name_val = esp_matter_char_str(product_name, strlen(product_name));
        err = attribute::set_val(product_name_attr, &product_name_val);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "✓ Product Name set to: %s", product_name);
        }
    }

    // 5. Serial Number 설정
    attribute_t *serial_number_attr = attribute::get(basic_info_cluster, BasicInformation::Attributes::SerialNumber::Id);
    if (serial_number_attr) {
        const char* serial_number = "ESP32C3-SENSOR-001";
        esp_matter_attr_val_t serial_number_val = esp_matter_char_str(serial_number, strlen(serial_number));
        err = attribute::set_val(serial_number_attr, &serial_number_val);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "✓ Serial Number set to: %s", serial_number);
        }
    }

    // 6. Manufacturing Date 설정 (YYYYMMDD 형식)
    attribute_t *mfg_date_attr = attribute::get(basic_info_cluster, BasicInformation::Attributes::ManufacturingDate::Id);
    if (mfg_date_attr) {
        const char* mfg_date = "20241201";
        esp_matter_attr_val_t mfg_date_val = esp_matter_char_str(mfg_date, strlen(mfg_date));
        err = attribute::set_val(mfg_date_attr, &mfg_date_val);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "✓ Manufacturing Date set to: %s", mfg_date);
        }
    }

    // 7. Part Number 설정
    attribute_t *part_number_attr = attribute::get(basic_info_cluster, BasicInformation::Attributes::PartNumber::Id);
    if (part_number_attr) {
        const char* part_number = "ESP32-C3-WROOM-02";
        esp_matter_attr_val_t part_number_val = esp_matter_char_str(part_number, strlen(part_number));
        err = attribute::set_val(part_number_attr, &part_number_val);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "✓ Part Number set to: %s", part_number);
        }
    }

    // 8. Hardware Version 설정
    attribute_t *hw_version_attr = attribute::get(basic_info_cluster, BasicInformation::Attributes::HardwareVersion::Id);
    if (hw_version_attr) {
        esp_matter_attr_val_t hw_version_val = esp_matter_uint16(1);
        err = attribute::set_val(hw_version_attr, &hw_version_val);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "✓ Hardware Version set to: 1");
        }
    }

    // 9. Hardware Version String 설정
    attribute_t *hw_version_str_attr = attribute::get(basic_info_cluster, BasicInformation::Attributes::HardwareVersionString::Id);
    if (hw_version_str_attr) {
        const char* hw_version_str = "v1.0";
        esp_matter_attr_val_t hw_version_str_val = esp_matter_char_str(hw_version_str, strlen(hw_version_str));
        err = attribute::set_val(hw_version_str_attr, &hw_version_str_val);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "✓ Hardware Version String set to: %s", hw_version_str);
        }
    }

    // 10. Software Version 설정
    attribute_t *sw_version_attr = attribute::get(basic_info_cluster, BasicInformation::Attributes::SoftwareVersion::Id);
    if (sw_version_attr) {
        esp_matter_attr_val_t sw_version_val = esp_matter_uint32(1);
        err = attribute::set_val(sw_version_attr, &sw_version_val);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "✓ Software Version set to: 1");
        }
    }

    // 11. Software Version String 설정
    attribute_t *sw_version_str_attr = attribute::get(basic_info_cluster, BasicInformation::Attributes::SoftwareVersionString::Id);
    if (sw_version_str_attr) {
        const char* sw_version_str = "v1.0.0";
        esp_matter_attr_val_t sw_version_str_val = esp_matter_char_str(sw_version_str, strlen(sw_version_str));
        err = attribute::set_val(sw_version_str_attr, &sw_version_str_val);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "✓ Software Version String set to: %s", sw_version_str);
        }
    }

    ESP_LOGI(TAG, "Basic Information configuration completed successfully");
    return ESP_OK;
}

// Identification callback 함수 (센서용)
static esp_err_t app_identification_cb(identification::callback_type_t type, uint16_t endpoint_id, 
                                      uint8_t effect_id, uint8_t effect_variant, void *priv_data)
{
    ESP_LOGI(TAG, "Sensor Identification callback: type=%u, endpoint=%u, effect=%u, variant=%u", 
             type, endpoint_id, effect_id, effect_variant);
    return ESP_OK;
}

// Attribute update callback 함수 (센서용)
static esp_err_t app_attribute_update_cb(attribute::callback_type_t type, uint16_t endpoint_id, 
                                        uint32_t cluster_id, uint32_t attribute_id, 
                                        esp_matter_attr_val_t *val, void *priv_data)
{
    esp_err_t err = ESP_OK;

    if (type == PRE_UPDATE) {
        // 센서 데이터 업데이트 처리
        ESP_LOGI(TAG, "Sensor attribute update: endpoint=%u, cluster=0x%08lX, attribute=0x%08lX", 
                 endpoint_id, cluster_id, attribute_id);
        
        // 여기에 실제 센서 하드웨어 제어 코드 추가
        // app_driver_handle_t driver_handle = (app_driver_handle_t)priv_data;
        // err = app_driver_attribute_update(driver_handle, endpoint_id, cluster_id, attribute_id, val);
    }

    return err;
}

// 센서 예제 메인 함수 (기존 센서 예제 app_main에 적용)
extern "C" void app_main()
{
    esp_err_t err = ESP_OK;

    ESP_LOGI(TAG, "Starting ESP32-C3 Matter Sensor with Basic Information");

    /* NVS 초기화 */
    nvs_flash_init();

    /* 센서 드라이버 초기화 (기존 코드 유지) */
    // app_driver_handle_t sensor_handle = app_driver_sensor_init();
    // app_driver_handle_t button_handle = app_driver_button_init();
    // app_reset_button_register(button_handle);

    /* Matter 노드 생성 */
    node::config_t node_config;
    node_t *node = node::create(&node_config, app_attribute_update_cb, app_identification_cb);
    ABORT_APP_ON_FAILURE(node != nullptr, ESP_LOGE(TAG, "Failed to create Matter node"));

    // ⭐ 여기서 센서 엔드포인트 생성 (기존 센서 예제와 동일)
    // 예시: Temperature Sensor 생성
    temperature_sensor::config_t temp_sensor_config;
    endpoint_t *endpoint = temperature_sensor::create(node, &temp_sensor_config, ENDPOINT_FLAG_NONE, nullptr);
    ABORT_APP_ON_FAILURE(endpoint != nullptr, ESP_LOGE(TAG, "Failed to create temperature sensor endpoint"));
    
    sensor_endpoint_id = endpoint::get_id(endpoint);
    ESP_LOGI(TAG, "Temperature sensor created with endpoint_id %d", sensor_endpoint_id);

    // ⭐ 중요: Matter 시작 전에 Basic Information 설정
    ESP_LOGI(TAG, "Configuring Basic Information before starting Matter...");
    err = set_basic_information_for_sensor(node);
    ABORT_APP_ON_FAILURE(err == ESP_OK, ESP_LOGE(TAG, "Failed to set basic information"));

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
    /* OpenThread 설정 */
    esp_openthread_platform_config_t config = {
        .radio_config = ESP_OPENTHREAD_DEFAULT_RADIO_CONFIG(),
        .host_config = ESP_OPENTHREAD_DEFAULT_HOST_CONFIG(),
        .port_config = ESP_OPENTHREAD_DEFAULT_PORT_CONFIG(),
    };
    set_openthread_platform_config(&config);
#endif

    /* Matter 시작 */
    ESP_LOGI(TAG, "Starting Matter stack...");
    err = esp_matter::start(app_event_cb);
    ABORT_APP_ON_FAILURE(err == ESP_OK, ESP_LOGE(TAG, "Failed to start Matter stack"));

    ESP_LOGI(TAG, "ESP32-C3 Matter Sensor started successfully!");
    ESP_LOGI(TAG, "Device ready for commissioning with configured Basic Information");

#if CONFIG_ENABLE_CHIP_SHELL
    esp_matter::console::diagnostics_register_commands();
    esp_matter::console::wifi_register_commands();
    esp_matter::console::factoryreset_register_commands();
#if CONFIG_OPENTHREAD_CLI
    esp_matter::console::otcli_register_commands();
#endif
    esp_matter::console::init();
#endif

    // 센서 초기값 설정 (예시)
    // app_driver_sensor_set_defaults(sensor_endpoint_id);
}

/*
 * 센서 예제에 통합하는 방법:
 * 
 * 1. 기존 센서 예제의 app_main.cpp를 엽니다
 * 2. set_basic_information_for_sensor() 함수를 추가합니다
 * 3. 노드와 엔드포인트 생성 후, esp_matter::start() 호출 전에 
 *    set_basic_information_for_sensor(node)를 호출합니다
 * 4. 필요에 따라 Vendor ID, Product ID, 제품명 등을 수정합니다
 * 
 * 지원 센서 타입:
 * - temperature_sensor
 * - humidity_sensor  
 * - pressure_sensor
 * - flow_sensor
 * - occupancy_sensor
 * - contact_sensor
 * - light_sensor
 * 
 * 각 센서 타입에 맞게 endpoint 생성 부분을 수정하세요.
 */
#endif