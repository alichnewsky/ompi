/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 The Trustees of the University of Tennessee.
 *                         All rights reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */

#include "ompi_config.h"

#include "ompi/mca/crcp/crcp.h"
#include "ompi/mca/crcp/base/base.h"
#include "crcp_coord.h"

/*
 * Public string for version number
 */
const char *ompi_crcp_coord_component_version_string = 
"OMPI CRCP coord MCA component version " OMPI_VERSION;

bool timing_enabled = false;

/*
 * Local functionality
 */
static int crcp_coord_open(void);
static int crcp_coord_close(void);

/*
 * Instantiate the public struct with all of our public information
 * and pointer to our public functions in it
 */
ompi_crcp_coord_component_t mca_crcp_coord_component = {
    /* First do the base component stuff */
    {
        /* Handle the general mca_component_t struct containing 
         *  meta information about the component
         */
        {
            OMPI_CRCP_BASE_VERSION_1_0_0,
            /* Component name and version */
            "coord",
            OMPI_MAJOR_VERSION,
            OMPI_MINOR_VERSION,
            OMPI_RELEASE_VERSION,
            
            /* Component open and close functions */
            crcp_coord_open,
            crcp_coord_close
        },

        /* Next the MCA v1.0.0 component meta data */
        {
            /* The component is checkpoint ready */
            MCA_BASE_METADATA_PARAM_CHECKPOINT
        },
        
        /* Query Function */
        ompi_crcp_coord_component_query,
        /* Verbosity level */
        0,
        /* opal_output handler */
        -1,
        /* Default priority */
        10
    }
};

static int crcp_coord_open(void) 
{
    int val;

    /*
     * This should be the last componet to ever get used since
     * it doesn't do anything.
     */
    mca_base_param_reg_int(&mca_crcp_coord_component.super.crcp_version,
                           "priority",
                           "Priority of the CRCP coord component",
                           false, false,
                           mca_crcp_coord_component.super.priority,
                           &mca_crcp_coord_component.super.priority);
    
    mca_base_param_reg_int(&mca_crcp_coord_component.super.crcp_version,
                           "verbose",
                           "Verbose level for the CRCP coord component",
                           false, false,
                           mca_crcp_coord_component.super.verbose, 
                           &mca_crcp_coord_component.super.verbose);
    /* If there is a custom verbose level for this component than use it
     * otherwise take our parents level and output channel
     */
    if ( 0 != mca_crcp_coord_component.super.verbose) {
        mca_crcp_coord_component.super.output_handle = opal_output_open(NULL);
        opal_output_set_verbosity(mca_crcp_coord_component.super.output_handle,
                                  mca_crcp_coord_component.super.verbose);
    } else {
        mca_crcp_coord_component.super.output_handle = ompi_crcp_base_output;
    }

    mca_base_param_reg_int(&mca_crcp_coord_component.super.crcp_version,
                           "timing",
                           "Enable Performance timing",
                           false, false,
                           0,
                           &val);
    if( 0 != val ) {
        timing_enabled = true;
    }
    else {
        timing_enabled = false;
    }

    /*
     * Debug Output
     */
    opal_output_verbose(10, mca_crcp_coord_component.super.output_handle,
                        "crcp:coord: open()");
    opal_output_verbose(20, mca_crcp_coord_component.super.output_handle,
                        "crcp:coord: open: priority   = %d", 
                        mca_crcp_coord_component.super.priority);
    opal_output_verbose(20, mca_crcp_coord_component.super.output_handle,
                        "crcp:coord: open: verbosity  = %d", 
                        mca_crcp_coord_component.super.verbose);

    return OMPI_SUCCESS;
}

static int crcp_coord_close(void)
{
    opal_output_verbose(10, mca_crcp_coord_component.super.output_handle,
                        "crcp:coord: close()");

    return OMPI_SUCCESS;
}
