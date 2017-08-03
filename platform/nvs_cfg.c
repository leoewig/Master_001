/**
 * NVS feature configuration
 *
 * @file
 * @ingroup nvs
 *
 * $Id: nvs_cfg.c 11203 2013-11-06 17:40:53Z vs7 $
 */
/* Copyright (c) Linkedriving Tech 2012 */
#include "basic_types.h"
#include "nvs.h"
#include "nvs_cfg.h"
#include "nvs_p.h"
#include "log.h"
#include "fm.h"
//#include "obd.h"
#include "mdm.h"
//#include "ctc.h"
//#include "ctc_p.h"
//#include "cmd.h"
#include "rtc.h"
#include "cal.h"

#include "rtc.h"

/**
 * Macro for entering feature NV Ram information
 */
#define NVS_FEAT_INFO(feat_nvs_wrapper)    { \
                                    &((feat_nvs_wrapper).header), \
                                    sizeof(feat_nvs_wrapper), \
                                    (sizeof(feat_nvs_wrapper) \
                                    - sizeof(feat_nvs_wrapper.header)) \
                                    - sizeof(feat_nvs_wrapper.reserved_bytes) \
                               }

/**
 * Array describing each feature's NV RAM usage.
 * Order must correspond to entries in #Nvs_feature_list_e
 */
/**@lint -e{648}
 * Overflow in computing constant for operation: 'unsigned sub.'
 *     [MISRA 2004 Rule 12.11]
 *
 * Lint doesn't lay structures out in the same way as Cosmic. It also
 * assumes that enums occupy 2 bytes even if the range of enumerators
 * would permit a smaller type. It can therefore calculate a different
 * size for types than the one Cosmic will actually use. There are no
 * options that can persuade lint to do things the Cosmic way.
 *
 * When lint calculates sizeof(feat_nvs_wrapper.reserved_bytes) it has
 * to subtract the number of bytes used by the feature from the number
 * available. The number of used bytes used is usually defined in terms
 * of the size of the structure type that forms the body of the feature's
 * NVRAM image. This subtraction can overflow if lint thinks that the
 * structure is larger than it is actually made by Cosmic.
 *
 * It's acceptable to suppress this message because Cosmic will already
 * have complained about the size of the reserved bytes being negative
 */
/*lint -e{648} */
FROM nvs_feat_list_s nvs_feat_list[Nvs_Max_feature] = {
                                            //NVS_FEAT_INFO(log_ram_image),
                                            //NVS_FEAT_INFO(fm_ram_image),
                                            NVS_FEAT_INFO(mdm_ram_image),
                                            //NVS_FEAT_INFO(obd_ram_image),
                                            //NVS_FEAT_INFO(umc_ram_image),                                          
                                            //NVS_FEAT_INFO(ctc_ram_image),
                                            //NVS_FEAT_INFO(cmd_ram_image),
                                            NVS_FEAT_INFO(cal_ram_image),
                                            NVS_FEAT_INFO(rtc_ram_image)
                                        };