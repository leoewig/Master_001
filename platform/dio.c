/**
 * @file
 *
 * Digital I/O driver
 *
 * $Id: dio.c 11351 2013-11-19 17:41:32Z pr4 $
 */

/**
 * @ingroup dio
 * @{
 * @file
 * @}
 */
#include "hardware.h"
#include "basic_types.h"
#include "dio.h"

/** PUPAE bit mask in PUCR register */
#define PUCR_PUPAE  0x01u
/** PUPBE bit mask in PUCR register */
#define PUCR_PUPBE  0x02u
/** PUPEE bit mask in PUCR register */
#define PUCR_PUPEE  0x10u
/** PUPKE bit mask in PUCR register */
#define PUCR_PUPKE  0x80u

/*
 * documented in header
 */
CONST struct dio_hw_mapping_s dio_mapping[] = {
/* output_portp  input_portp    ddrp    pull en / dis   pull polarity    per pin / per port      pull per port bit mask */
  {   &PORTA,       &PORTA,     &DDRA,  &PUCR,          NULL,            Dio_pull_per_port,      PUCR_PUPAE},
  {   &PORTB,       &PORTB,     &DDRB,  &PUCR,          NULL,            Dio_pull_per_port,      PUCR_PUPBE},
  {   &PORTE,       &PORTE,     &DDRE,  &PUCR,          NULL,            Dio_pull_per_port,      PUCR_PUPEE},
  {   &PTH,         &PTIH,      &DDRH,  &PERH,          &PPSH,           Dio_pull_per_pin,       0x00u},
  {   &PTJ,         &PTIJ,      &DDRJ,  &PERJ,          &PPSJ,           Dio_pull_per_pin,       0x00u},
  {   &PORTK,       &PORTK,     &DDRK,  &PUCR,          NULL,            Dio_pull_per_port,      PUCR_PUPKE},
  {   &PTM,         &PTM,       &DDRM,  &PERM,          &PPSM,           Dio_pull_per_pin,       0x00u},
  {   &PTP,         &PTIP,      &DDRP,  &PERP,          &PPSP,           Dio_pull_per_pin,       0x00u},
  {   &PTT,         &PTIT,      &DDRT,  &PERT,          &PPST,           Dio_pull_per_pin,       0x00u},
  {   &PTS,         &PTIS,      &DDRS,  &PERS,          &PPSS,           Dio_pull_per_pin,       0x00u},
};

/**
 * @par Implementation
 *
 * Sets DDR register bits for digital outputs and clears them for inputs.
 *
 * Uses #dio_mapping[] to get the data direction register for the port.
 *
 * Drives all outputs to their inactive state to avoid glitching an active state
 * if they are active-low.
 *
 * Throw an internal error if the interface is Dio_interface_function and an
 * output variable is NULL.
 *
 * Reads back the values on the pins and sets the dmd variables.
 *
 * @note If the ADC digital ports are used, this function will have to set the
 * DIEN digital enable register for both inputs and outputs.  This is probably
 * best done with explicit code <tt>if (cfgp->port == PT0AD0)...</tt> rather
 * than by augmenting the dio_mapping array
 */
void dio_init(void)
{
    CONST struct dio_in_cfg_s *cfg_in_p; /* pointer to ip channel config */
    CONST struct dio_out_cfg_s *cfg_out_p; /* pointer to op channel config */
    CONST struct dio_hw_mapping_s *hw_mapping; /* pointer to h/w info */
    CONST struct dio_pull_cfg_s *cfg_pull_p; /* pointer to pull resistor config */
    bool_t pinlevel;    /* state of pin: high -> TRUE, low -> FALSE */
    uint8_t ccr;    /* CCR byte for interrupt save/restore */

    uint8_t i;

    /* ECLK and ECLKX2 overlaps with PORTE, this port is owned by DIO so
     * disable the ECLK device (ECLKX2 should be disabled out of reset) */
    DIO_DISABLE_ECLK();

    for (i = 0u ; i < dio_n_outputs; i++)
    {
        cfg_out_p = &dio_output_cfg[i];

        if ((cfg_out_p->interface == Dio_interface_function) &&
            (cfg_out_p->var == NULL))
        {
            /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
            INTERNAL_ERROR();
        }

        hw_mapping = &dio_mapping[cfg_out_p->port];

        /*
         * calculate the desired pin state depending
         * on whether the output is active high or low
         */
        pinlevel = (cfg_out_p->active_high_low == Dio_active_high)? FALSE : TRUE;
        if (pinlevel == TRUE)
        {
            SAVE_INTERRUPTS(ccr);
            *hw_mapping->output_portp |= (uint8_t) (1u << cfg_out_p->bitnum);
            RESTORE_INTERRUPTS(ccr);
        }
        else
        {
            SAVE_INTERRUPTS(ccr);
            *hw_mapping->output_portp &=
                        (uint8_t) ~(uint8_t) (1u << cfg_out_p->bitnum);
            RESTORE_INTERRUPTS(ccr);
        }

        /* set the data direction register to 'output' */
        *hw_mapping->ddrp |= (uint8_t) (1u << cfg_out_p->bitnum);

        /* Set most recently requested level */
        if (cfg_out_p->dmd != NULL)
        {
            *cfg_out_p->dmd = pinlevel;
        }

        /* Read back the value on the pin */
        /** @polyspace:begin<MISRA-C:13.2:Low:Investigate> Could code be changed to fix this? */
        if (cfg_out_p->read_back != NULL)
        {
            *cfg_out_p->read_back = (*hw_mapping->input_portp
                          & (uint8_t) (1u << cfg_out_p->bitnum)) ?
                          (cfg_out_p->active_high_low == Dio_active_high) :
                          (cfg_out_p->active_high_low == Dio_active_low);
        }
        /** @polyspace:end<MISRA-C:13.2:Low:Investigate> Could code be changed to fix this? */

    }

    for (i = 0u ; i < dio_n_inputs; i++)
    {
        cfg_in_p = &dio_input_cfg[i];

        hw_mapping = &dio_mapping[cfg_in_p->port];

        if ((cfg_in_p->interface == Dio_interface_function) &&
            (cfg_in_p->var == NULL))
        {
            /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
            INTERNAL_ERROR();
        }

        /* set the data direction register to 'input' */
        *hw_mapping->ddrp &= (uint8_t) ~ (uint8_t) (1u << cfg_in_p->bitnum);
    }

    for (i = 0u ; i < dio_n_pull_cfgs; i++)
    {
        cfg_pull_p = &dio_pull_cfg[i];
        hw_mapping = &dio_mapping[cfg_pull_p->port];

        if((hw_mapping->pull_per == Dio_pull_per_port) && (cfg_pull_p->bitmask != 0xffu))
        {
            /* pull resistor bit mask of ports, which may be configured per port basis only
             * must be equal to 0xffu. */
            /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
            INTERNAL_ERROR();
        }
        if((hw_mapping->pull_polarity == NULL) && (cfg_pull_p->pull_cfg == Dio_pull_down))
        {
            /* ports with polarity register not specified may be enabled with pull up device only */
            /** @polyspace<MISRA-C:14.2:Not a defect:Justify with annotations> For test purposes only.*/
            INTERNAL_ERROR();
        }
        if(cfg_pull_p->pull_cfg == Dio_pull_default)
        {
            /* do nothing */
        }
        else if(cfg_pull_p->pull_cfg == Dio_pull_none)
        {
            /* disable pull resistors */
            if(hw_mapping->pull_per == Dio_pull_per_pin)
            {
                *hw_mapping->pull_cfg &= (uint8_t) ~ (uint8_t) cfg_pull_p->bitmask;
            }
            else
            {
                *hw_mapping->pull_cfg &= (uint8_t) ~ (uint8_t) hw_mapping->pull_per_port_mask;
            }
        }
        else
        {
            /* enable pull resistors */
            if(hw_mapping->pull_per == Dio_pull_per_pin)
            {
                *hw_mapping->pull_cfg |= cfg_pull_p->bitmask;
            }
            else
            {
                *hw_mapping->pull_cfg |= hw_mapping->pull_per_port_mask;
            }
            /* configure polarity if polarity register is defined */
            if(hw_mapping->pull_polarity)
            {
                if(cfg_pull_p->pull_cfg == Dio_pull_down)
                {
                    *hw_mapping->pull_polarity |= cfg_pull_p->bitmask;
                }
                else
                {
                    *hw_mapping->pull_polarity &= (uint8_t) ~ (uint8_t) cfg_pull_p->bitmask;
                }
            }
        }
    }
}

void dio_read_inputs(void)
{
    uint8_t i;
    bool_t pinlevel;    /* state of pin: high -> TRUE, low -> FALSE */
    CONST struct dio_in_cfg_s *cfgp; /* pointer to channel configuration */
    CONST struct dio_hw_mapping_s *hw_mapping; /* pointer to h/w info */

    for (i = 0u ; i < dio_n_inputs ; i++)
    {
        if (dio_input_cfg[i].interface == Dio_interface_function)
        {
            /* handled by this function so read pin */
            cfgp = &dio_input_cfg[i];    /* config for this input line */
            hw_mapping = &dio_mapping[cfgp->port]; /* h/w registers for this
                                  port */

            /* read the state of the pin */
            pinlevel = ((*hw_mapping->input_portp
                    & (uint8_t) (1u << cfgp->bitnum)) != FALSE) ? TRUE : FALSE;
            /*
             * write the application boolean variable pointed to by cfgp->var,
             * inverting the pin level if channel is active low
             */
            if (cfgp->active_high_low == Dio_active_high)
            {
                *cfgp->var = pinlevel;
            }
            else
            {
                *cfgp->var = (pinlevel == TRUE)? FALSE:TRUE;
            }
        }
        else
        {
            /* this one handled elsewhere, so do nothing */
        }
    }
}

/**
 * @par Implementation
 *
 * Interrupts are disabled around the read-modify-write for each output port.
 * This will prevent corruption of writes by higher-priority tasks on the S12
 * but will not prevent possible corruption if the XGATE writes to the same
 * port.
 */
void dio_write_outputs(void)
{
    uint8_t i;
    uint8_t ccr;    /* CCR byte for interrupt save/restore */
    bool_t pinlevel;
    CONST struct dio_out_cfg_s *cfgp; /* pointer to channel configuration */
    CONST struct dio_hw_mapping_s *hw_mapping; /* pointer to h/w info */

    for (i = 0u ; i < dio_n_outputs ; i++)
    {
        if (dio_output_cfg[i].interface == Dio_interface_function)
        {
            /* handled here so write pin */
            cfgp = &dio_output_cfg[i];    /* config for this output line */
            hw_mapping = &dio_mapping[cfgp->port]; /* h/w registers for this
                                                      port */

            /*
             * Check to see if the overrride calibration is enabled.
             * If so set the output to the override value (cfgp->override_val),
             * set to user application desired value (cfgp->var) otherwise.
             */
            if ((cfgp->override_en != FNULL) &&
                (cfgp->override_val != FNULL) &&
                (*cfgp->override_en == TRUE))
            {
                pinlevel = *cfgp->override_val;
            }
            else
            {
                pinlevel = *cfgp->var;
            }

            /*
             * Take active level configuration into account
             */
            if (cfgp->active_high_low == Dio_active_low)
            {
                /* invert the level */
                if(pinlevel == TRUE)
                {
                    pinlevel = FALSE;
                }
                else
                {
                    pinlevel = TRUE;
                }
            }
            else
            {
                /* no need to invert */
            }

            if (pinlevel == TRUE)
            {
                SAVE_INTERRUPTS(ccr);
                *hw_mapping->output_portp |= (uint8_t) (1u << cfgp->bitnum);
                RESTORE_INTERRUPTS(ccr);
            }
            else
            {
                SAVE_INTERRUPTS(ccr);
                *hw_mapping->output_portp &=
                            (uint8_t) ~(uint8_t) (1u << cfgp->bitnum);
                RESTORE_INTERRUPTS(ccr);
            }

            /* Set most recently requested level */
            if (cfgp->dmd != NULL)
            {
                *cfgp->dmd = pinlevel;
            }

            /* Read back the value on the pin */
            /** @polyspace:begin<MISRA-C:13.2:Low:Investigate> Could code be changed to fix this? */
            if (cfgp->read_back != NULL)
            {
                *cfgp->read_back = (*hw_mapping->input_portp
                                    & (uint8_t) (1u << cfgp->bitnum)) ?
                                   (cfgp->active_high_low == Dio_active_high) :
                                   (cfgp->active_high_low == Dio_active_low);
            }
            /** @polyspace:end<MISRA-C:13.2:Low:Investigate> Could code be changed to fix this? */
        }
        else
        {
            /* this one handled elsewhere, so do nothing */
        }
    }
}
