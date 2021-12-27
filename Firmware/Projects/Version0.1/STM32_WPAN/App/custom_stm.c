/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_stm.c
  * @author  MCD Application Team
  * @brief   Custom Example Service.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "common_blesvc.h"
#include "custom_stm.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  uint16_t  CustomLsHdle;                    /**< LocationService handle */
  uint16_t  CustomLnfHdle;                  /**< LNFeature handle */
  uint16_t  CustomLscHdle;                  /**< LocationandSpeed handle */
  uint16_t  CustomBatservHdle;                    /**< BatteryService handle */
  uint16_t  CustomBatlvlHdle;                  /**< BatteryLevel handle */
}CustomContext_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
#define UUID_128_SUPPORTED  1

#if (UUID_128_SUPPORTED == 1)
#define BM_UUID_LENGTH  UUID_TYPE_128
#else
#define BM_UUID_LENGTH  UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE    (3)

/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
#define CHARACTERISTIC_DESCRIPTOR_ATTRIBUTE_OFFSET         2
#define CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET              1
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
static const uint8_t SizeLnf=1;
static const uint8_t SizeLsc=1;
static const uint8_t SizeBatlvl=1;
/**
 * START of Section BLE_DRIVER_CONTEXT
 */
PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static CustomContext_t CustomContext;

/**
 * END of Section BLE_DRIVER_CONTEXT
 */

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *pckt);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
/* USER CODE BEGIN PFD */

/* USER CODE END PFD */

/* Private functions ----------------------------------------------------------*/

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Characteristics Service */
/*
 The following 128bits UUIDs have been generated from the random UUID
 generator:
 D973F2E0-B19E-11E2-9E96-0800200C9A66: Service 128bits UUID
 D973F2E1-B19E-11E2-9E96-0800200C9A66: Characteristic_1 128bits UUID
 D973F2E2-B19E-11E2-9E96-0800200C9A66: Characteristic_2 128bits UUID
 */

/* USER CODE BEGIN PF */

/* USER CODE END PF */

/**
 * @brief  Event handler
 * @param  Event: Address of the buffer holding the Event
 * @retval Ack: Return whether the Event has been managed or not
 */
static SVCCTL_EvtAckStatus_t Custom_STM_Event_Handler(void *Event)
{
  SVCCTL_EvtAckStatus_t return_value;
  hci_event_pckt *event_pckt;
  evt_blecore_aci *blecore_evt;
  aci_gatt_read_permit_req_event_rp0    *read_req;
  Custom_STM_App_Notification_evt_t     Notification;
  /* USER CODE BEGIN Custom_STM_Event_Handler_1 */

  /* USER CODE END Custom_STM_Event_Handler_1 */

  return_value = SVCCTL_EvtNotAck;
  event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);

  switch(event_pckt->evt)
  {
    case HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE:
      blecore_evt = (evt_blecore_aci*)event_pckt->data;
      switch(blecore_evt->ecode)
      {
        case ACI_GATT_ATTRIBUTE_MODIFIED_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_BEGIN */
          /* USER CODE BEGIN EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */

          /* USER CODE END EVT_BLUE_GATT_ATTRIBUTE_MODIFIED_END */
          break;

        case ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE :
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_BEGIN */
          read_req = (aci_gatt_read_permit_req_event_rp0*)blecore_evt->data;
          if(read_req->Attribute_Handle == (CustomContext.CustomLnfHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */

            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_1_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if(read_req->Attribute_Handle == (CustomContext.CustomLnfHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if(read_req->Attribute_Handle == (CustomContext.CustomLscHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */

            /*USER CODE END CUSTOM_STM_Service_1_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_1_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_1_Char_2_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if(read_req->Attribute_Handle == (CustomContext.CustomLscHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          else if(read_req->Attribute_Handle == (CustomContext.CustomBatlvlHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))
          {
            return_value = SVCCTL_EvtAckFlowEnable;
            /*USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1 */

            /*USER CODE END CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_1*/
            aci_gatt_allow_read(read_req->Connection_Handle);
            /*USER CODE BEGIN CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2 */

            /*USER CODE END CUSTOM_STM_Service_2_Char_1_ACI_GATT_READ_PERMIT_REQ_VSEVT_CODE_2*/
          } /* if(read_req->Attribute_Handle == (CustomContext.CustomBatlvlHdle + CHARACTERISTIC_VALUE_ATTRIBUTE_OFFSET))*/
          /* USER CODE BEGIN EVT_BLUE_GATT_READ_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_READ_PERMIT_REQ_END */
          break;

        case ACI_GATT_WRITE_PERMIT_REQ_VSEVT_CODE:
          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_BEGIN */
          /* USER CODE BEGIN EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */

          /* USER CODE END EVT_BLUE_GATT_WRITE_PERMIT_REQ_END */
          break;
        /* USER CODE BEGIN BLECORE_EVT */

        /* USER CODE END BLECORE_EVT */
        default:
          /* USER CODE BEGIN EVT_DEFAULT */

          /* USER CODE END EVT_DEFAULT */
          break;
      }
      /* USER CODE BEGIN EVT_VENDOR*/

      /* USER CODE END EVT_VENDOR*/
      break; /* HCI_VENDOR_SPECIFIC_DEBUG_EVT_CODE */

      /* USER CODE BEGIN EVENT_PCKT_CASES*/

      /* USER CODE END EVENT_PCKT_CASES*/

    default:
      /* USER CODE BEGIN EVENT_PCKT*/

      /* USER CODE END EVENT_PCKT*/
      break;
  }

  /* USER CODE BEGIN Custom_STM_Event_Handler_2 */

  /* USER CODE END Custom_STM_Event_Handler_2 */

  return(return_value);
}/* end Custom_STM_Event_Handler */

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Service initialization
 * @param  None
 * @retval None
 */
void SVCCTL_InitCustomSvc(void)
{

  Char_UUID_t  uuid;
  /* USER CODE BEGIN SVCCTL_InitCustomSvc_1 */

  /* USER CODE END SVCCTL_InitCustomSvc_1 */

  /**
   *  Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Custom_STM_Event_Handler);

  /*
   *          LocationService
   *
   * Max_Attribute_Records = 1 + 2*2 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for LocationService +
   *                                2 for LNFeature +
   *                                2 for LocationandSpeed +
   *                                1 for LNFeature broadcast property +
   *                                1 for LocationandSpeed broadcast property +
   *                              = 7
   */

  uuid.Char_UUID_16 = 0x1819;
  aci_gatt_add_service(UUID_TYPE_16,
                       (Service_UUID_t *) &uuid,
                       PRIMARY_SERVICE,
                       7,
                       &(CustomContext.CustomLsHdle));

  /**
   *  LNFeature
   */
  uuid.Char_UUID_16 = 0x2a6a;
  aci_gatt_add_char(CustomContext.CustomLsHdle,
                    UUID_TYPE_16, &uuid,
                    SizeLnf,
                    CHAR_PROP_BROADCAST | CHAR_PROP_READ,
                    ATTR_PERMISSION_NONE,
                    GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                    0x10,
                    CHAR_VALUE_LEN_CONSTANT,
                    &(CustomContext.CustomLnfHdle));
  /**
   *  LocationandSpeed
   */
  uuid.Char_UUID_16 = 0x2a67;
  aci_gatt_add_char(CustomContext.CustomLsHdle,
                    UUID_TYPE_16, &uuid,
                    SizeLsc,
                    CHAR_PROP_BROADCAST | CHAR_PROP_READ,
                    ATTR_PERMISSION_NONE,
                    GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                    0x10,
                    CHAR_VALUE_LEN_CONSTANT,
                    &(CustomContext.CustomLscHdle));

  /*
   *          BatteryService
   *
   * Max_Attribute_Records = 1 + 2*1 + 1*no_of_char_with_notify_or_indicate_property + 1*no_of_char_with_broadcast_property
   * service_max_attribute_record = 1 for BatteryService +
   *                                2 for BatteryLevel +
   *                              = 3
   */

  uuid.Char_UUID_16 = 0x180f;
  aci_gatt_add_service(UUID_TYPE_16,
                       (Service_UUID_t *) &uuid,
                       PRIMARY_SERVICE,
                       3,
                       &(CustomContext.CustomBatservHdle));

  /**
   *  BatteryLevel
   */
  uuid.Char_UUID_16 = 0x2a19;
  aci_gatt_add_char(CustomContext.CustomBatservHdle,
                    UUID_TYPE_16, &uuid,
                    SizeBatlvl,
                    CHAR_PROP_READ,
                    ATTR_PERMISSION_NONE,
                    GATT_NOTIFY_ATTRIBUTE_WRITE | GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP,
                    0x10,
                    CHAR_VALUE_LEN_CONSTANT,
                    &(CustomContext.CustomBatlvlHdle));

  /* USER CODE BEGIN SVCCTL_InitCustomSvc_2 */

  /* USER CODE END SVCCTL_InitCustomSvc_2 */

  return;
}

/**
 * @brief  Characteristic update
 * @param  CharOpcode: Characteristic identifier
 * @param  Service_Instance: Instance of the service to which the characteristic belongs
 *
 */
tBleStatus Custom_STM_App_Update_Char(Custom_STM_Char_Opcode_t CharOpcode, uint8_t *pPayload)
{
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  /* USER CODE BEGIN Custom_STM_App_Update_Char_1 */

  /* USER CODE END Custom_STM_App_Update_Char_1 */

  switch(CharOpcode)
  {

    case CUSTOM_STM_LNF:
      result = aci_gatt_update_char_value(CustomContext.CustomLsHdle,
                                          CustomContext.CustomLnfHdle,
                                          0, /* charValOffset */
                                          SizeLnf, /* charValueLen */
                                          (uint8_t *)  pPayload);
      /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_1*/

      /* USER CODE END CUSTOM_STM_Service_1_Char_1*/
      break;

    case CUSTOM_STM_LSC:
      result = aci_gatt_update_char_value(CustomContext.CustomLsHdle,
                                          CustomContext.CustomLscHdle,
                                          0, /* charValOffset */
                                          SizeLsc, /* charValueLen */
                                          (uint8_t *)  pPayload);
      /* USER CODE BEGIN CUSTOM_STM_Service_1_Char_2*/

      /* USER CODE END CUSTOM_STM_Service_1_Char_2*/
      break;

    case CUSTOM_STM_BATLVL:
      result = aci_gatt_update_char_value(CustomContext.CustomBatservHdle,
                                          CustomContext.CustomBatlvlHdle,
                                          0, /* charValOffset */
                                          SizeBatlvl, /* charValueLen */
                                          (uint8_t *)  pPayload);
      /* USER CODE BEGIN CUSTOM_STM_Service_2_Char_1*/

      /* USER CODE END CUSTOM_STM_Service_2_Char_1*/
      break;

    default:
      break;
  }

  /* USER CODE BEGIN Custom_STM_App_Update_Char_2 */

  /* USER CODE END Custom_STM_App_Update_Char_2 */

  return result;
}
