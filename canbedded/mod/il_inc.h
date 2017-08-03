/* Kernbauer Version: 1.14 Konfiguration: Il_Vector Erzeugungsgangnummer: 50 */

/* STARTSINGLE_OF_MULTIPLE */
/* KB begin Il_Vector_HeaderText */
/* KB end Il_Vector_HeaderText */
/**********************************************************************************************************************
  COPYRIGHT
-----------------------------------------------------------------------------------------------------------------------
  \par      copyright
  \verbatim
  Copyright (c) 2000-2008 by Vector Informatik GmbH. All rights reserved.
 
                This software is copyright protected and proprietary to Vector Informatik GmbH.
                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
                All other rights remain with Vector Informatik GmbH.
  \endverbatim
-----------------------------------------------------------------------------------------------------------------------
  FILE DESCRIPTION
-----------------------------------------------------------------------------------------------------------------------
  \file  File:  Il_Inc.h
      Project:  Il_Vector
       Module:  -
    Generator:  -

  \brief Description:  Vector Interaction Layer include header file
**********************************************************************************************************************/

/**********************************************************************************************************************
  AUTHOR IDENTITY
-----------------------------------------------------------------------------------------------------------------------
  Name                          Initials      Company
-----------------------------------------------------------------------------------------------------------------------
  Heike Honert                  Ht            Vector Informatik GmbH
  Georg Pfluegel                Pl            Vector Informatik GmbH
  Armin Happel                  Hp            Vector Informatik GmbH
  Gunnar Meiss                  Ms            Vector Informatik GmbH
  Ralf Fritz                    Fz            Vector Informatik GmbH
-----------------------------------------------------------------------------------------------------------------------
  REVISION HISTORY
-----------------------------------------------------------------------------------------------------------------------
  Version   Date        Author  Change Id     Description
-----------------------------------------------------------------------------------------------------------------------
  01.00.00  2001-04-18  Ht                    Creation
  01.01.00  2002-05-15  Pl                    add STARTSINGLE_OF_MULTIPLE and STOPSINGLE_OF_MULTIPLE
  01.02.00  2002-08-16  Ht                    avoid multiple include
  01.03.00  2003-07-18  Hp                    Include NM-Header for certain OEM's only.
  01.04.00  2004-08-18  Ms      ESCAN00009633 Naming Conventions
  01.05.00  2005-05-09  Ms      ESCAN00008746 Geny Compatibility
  01.06.00  2005-11-17  Ms      ESCAN00013712 added AUTOSAR Pdu Interface
  01.07.00  2006-04-10  Ms      ESCAN00016017 Add include for Il_Ni
  01.08.00  2007-03-30  Fz      ESCAN00020135 GM-Only: Include file added
  05.00.00  2008-01-10  Ms      ESCAN00023206 Split Oem Extensions
                        Ms      ESCAN00023270 No changes here
                        Ms      ESCAN00023300 No changes here
                        Ms      ESCAN00023284 No changes here
                        Ms      ESCAN00023821 No changes here
                        Ms      ESCAN00023822 GM-Only: No changes here
  05.01.00  2008-01-25  Ms      ESCAN00023109 PSA-Only: No changes here
  05.02.00  2008-04-21  Ms      ESCAN00026301 No changes here
            2008-07-17  Ms      ESCAN00025002 No changes here
                        Ms      ESCAN00028780 No changes here
                        Ms      ESCAN00025178 No changes here
                        Ms      ESCAN00013854 No changes here
  05.02.01  2008-10-17  Ms      ESCAN00029307 No changes here
                        Ms      ESCAN00030750 No changes here
**********************************************************************************************************************/

#if !defined ( V_IL_INC_COMPONENT_HEADER )
# define V_IL_INC_COMPONENT_HEADER
/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/

#  include "v_cfg.h"            /* Generated file...... */

#  if defined ( VGEN_GENY )
#   include "v_inc.h"           /* Generated file...... */
#  endif

#  include "il_cfg.h"

#  if !defined ( VGEN_GENY )
#  endif

#  if defined ( VGEN_GENY )
#   include "il_par.h"          /* Generated file...... */
#  endif

#  include "il_def.h"           /* Il_Vector definition file. */

#  if( IL_VECTOR_VERSION != 0x0502 )
#   error "Source and Header file are inconsistent!"
#  endif
#  if( IL_VECTOR_RELEASE_VERSION != 0x01 )
#   error "Source and Header file are inconsistent!"
#  endif



/**********************************************************************************************************************
  END OF FILE: il_inc.h
**********************************************************************************************************************/
#endif /* V_IL_INC_COMPONENT_HEADER */
/* STOPSINGLE_OF_MULTIPLE */
/************   Organi, Version 3.7.1 Vector-Informatik GmbH  ************/
