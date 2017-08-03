/******   STARTSINGLE_OF_MULTIPLE_COMMENT    ******/


/*****************************************************************************/
/**
 * @file  SIP_VERS.H
 * @brief Vector SIP (Software Integration Package) version check
 *
 *****************************************************************************
 *  Description: See comments in C file
 *
 *****************************************************************************
 *  Usage hints: See comments in C file
 *
 *****************************************************************************
 *               C O P Y R I G H T
 *****************************************************************************
 * Copyright (c) 2002 by Vector Informatik GmbH.           All rights reserved.
 *
 *                    Alle Rechte an der Software verbleiben bei der
 *                    Vector Informatik GmbH.
 *                    Vector Informatik GmbH räumt dem Lizenznehmer das unwider-
 *                    rufliche, geographisch und zeitlich nicht beschränkte,
 *                    jedoch nicht ausschließliche Nutzungsrecht innerhalb des
 *                    Lizenznehmers ein. Die Weitergabe des Nutzungsrechts
 *                    durch den Lizenznehmers ist auf dessen Zulieferer
 *                    beschränkt. Die Zulieferer sind zu verpflichten, die
 *                    Software nur im Rahmen von Projekten für den Lizenznehmer
 *                    zu verwenden; weitere Rechte der Zulieferer sind
 *                    auszuschließen.
 *
 *     Use of this source code is subject to the License Agreement "license.txt"
 *     distributed with this file. You may not use this source code unless you
 *     agree to the terms of the License Agreement.
 *
 *     REMOVAL OF THESE COMMENTS IS A VIOLATION OF THE LICENSE AGREEMENT.
 *
 *****************************************************************************
 * See SIP_VERS.C for version history!
 *****************************************************************************/

#ifndef  _SIP_VERSION_CHECK_
#define  _SIP_VERSION_CHECK_

/******************************************************************************
| SIP_VERS Version                  (abcd: Main version ab Sub Version cd)
|*****************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : Common_SipVersionCheck CQComponent : Implementation */
#define COMMON_SIPVERSIONCHECK_VERSION            0x0300
#define COMMON_SIPVERSIONCHECK_RELEASE_VERSION    0x37

/* Used to check source against header. This is NOT the SIP version! */
#define SIP_VERS_VERSION          COMMON_SIPVERSIONCHECK_VERSION
#define SIP_VERS_RELEASE_VERSION  COMMON_SIPVERSIONCHECK_RELEASE_VERSION


/******************************************************************************
| SIP Version                       (abcd: Main version ab Sub Version cd)
|*****************************************************************************/
#define VECTOR_SIP_VERSION          _VECTOR_SIP_VERSION
#define VECTOR_SIP_RELEASE_VERSION  _VECTOR_SIP_RELEASE_VERSION

/* ROM CATEGORY 4 START*/
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kSipMainVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kSipSubVersion;
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 kSipBugFixVersion;
/* ROM CATEGORY 4 END*/

#endif /* _SIP_VERSIONCHECK_ */
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
