/*--------------------------------------------------------------------
  (C) Copyright 2006-2012 Barcelona Supercomputing Center
                          Centro Nacional de Supercomputacion
  
  This file is part of Mercurium C/C++ source-to-source compiler.
  
  See AUTHORS file in the top level directory for information
  regarding developers and contributors.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  
  Mercurium C/C++ source-to-source compiler is distributed in the hope
  that it will be useful, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the GNU Lesser General Public License for more
  details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with Mercurium C/C++ source-to-source compiler; if
  not, write to the Free Software Foundation, Inc., 675 Mass Ave,
  Cambridge, MA 02139, USA.
--------------------------------------------------------------------*/




#include "cxx-typeorder.h"
#include "cxx-typededuc.h"
#include "cxx-typeutils.h"
#include "cxx-typeunif.h"
#include "cxx-utils.h"
#include "cxx-cexpr.h"
#include "cxx-exprtype.h"
#include "cxx-limits.h"

static char is_less_or_equal_specialized_template_conversion_function(
        type_t* f1, type_t* f2, 
        decl_context_t decl_context, template_parameter_list_t** deduced_template_arguments,
        const locus_t* locus);

static char is_less_or_equal_specialized_template_function_common_(type_t* f1, type_t* f2,
        decl_context_t decl_context, 
        template_parameter_list_t** deduced_template_arguments,
        template_parameter_list_t* explicit_template_parameters,
        const locus_t* locus, char is_conversion)
{
    if (is_conversion)
    {
        return is_less_or_equal_specialized_template_conversion_function(f1, f2,
                decl_context, deduced_template_arguments, locus);
    }

    ERROR_CONDITION(!is_function_type(f1) || !is_function_type(f2), "functions types are not", 0);

    DEBUG_CODE()
    {
        fprintf(stderr, "TYPEORDER: Computing whether one function type is less or equal specialized than the other\n");
        fprintf(stderr, "TYPEORDER: Is\n");
        fprintf(stderr, "TYPEORDER:      %s\n", print_declarator(f1));
        fprintf(stderr, "TYPEORDER: less or equal specialized than\n");
        fprintf(stderr, "TYPEORDER:      %s\n", print_declarator(f2));
        fprintf(stderr, "TYPEORDER: ?\n");
    }

    int num_arguments = function_type_get_num_parameters(f2);
    if (function_type_get_has_ellipsis(f2))
        num_arguments--;

    int num_parameters = function_type_get_num_parameters(f1);
    if (function_type_get_has_ellipsis(f1))
        num_parameters--;

    if (num_arguments != num_parameters)
    {
        DEBUG_CODE()
        {
            fprintf(stderr, "TYPEORDER: It is not less or equal specialized template function because number arguments != number parameters\n");
        }
        return 0;
    }

    type_t * arguments[MCXX_MAX_ARGUMENTS_FOR_DEDUCTION];
    type_t * parameters[MCXX_MAX_ARGUMENTS_FOR_DEDUCTION];
    type_t * original_parameters[MCXX_MAX_ARGUMENTS_FOR_DEDUCTION];

    int i;
    for (i = 0; i < num_arguments; i++)
    {
        ERROR_CONDITION(i >= MCXX_MAX_ARGUMENTS_FOR_DEDUCTION, 
                "Too many types for deduction", 0);
        arguments[i] = function_type_get_parameter_type_num(f2, i);
        original_parameters[i]
            = parameters[i]
            = function_type_get_parameter_type_num(f1, i);
    }

    // Try to deduce types of template type F1 using F2
    template_parameter_list_t* type_template_parameters = 
        template_specialized_type_get_template_parameters(f1);
    template_parameter_list_t* template_parameters = 
        template_specialized_type_get_template_arguments(f1);

    if (!deduce_template_arguments_common(
                template_parameters, type_template_parameters,
                arguments, num_arguments,
                parameters, num_parameters,
                original_parameters,
                decl_context,
                deduced_template_arguments,
                locus,
                explicit_template_parameters,
                /* is_function_call */ 0))
    {
        DEBUG_CODE()
        {
            fprintf(stderr, "TYPEORDER: No deduction was possible\n");
        }
        return 0;
    }

    decl_context_t updated_context = decl_context;
    updated_context.template_parameters = *deduced_template_arguments;

    for (i = 0; i < num_arguments; i++)
    {
        type_t* original_type = function_type_get_parameter_type_num(f1, i);

        type_t* updated_type = update_type(original_type,
                updated_context,
                locus);

        // Check the soundness of the updated type
        if (updated_type == NULL)
        {
            DEBUG_CODE()
            {
                fprintf(stderr, "TYPEORDER: The deduced type was not constructible\n");
                fprintf(stderr, "TYPEORDER: It is not less or equal specialized template function because type deduction failed\n");
            }
            return 0;
        }

        type_t* argument_type = function_type_get_parameter_type_num(f2, i);

        if (!equivalent_types(updated_type, argument_type))
        {
            DEBUG_CODE()
            {
                fprintf(stderr, "TYPEORDER: It is not less or equal specialized template function "
                        "because type of parameter (%s) and type of argument (%s) do not match\n",
                        print_declarator(updated_type),
                        print_declarator(argument_type));
            }
            return 0;
        }
    }

    DEBUG_CODE()
    {
        fprintf(stderr, "TYPEORDER: It IS less or equal specialized\n");
    }

    return 1;
}

char is_less_or_equal_specialized_template_class(type_t* c1, type_t* c2, 
        decl_context_t decl_context UNUSED_PARAMETER,
        template_parameter_list_t** deduced_template_arguments, const locus_t* locus)
{
    ERROR_CONDITION(!is_named_class_type(c1)
            || !is_named_class_type(c2)
            || !is_template_specialized_type(get_actual_class_type(c1))
            || !is_template_specialized_type(get_actual_class_type(c2)),
            "Specialized classes are not", 0);

    parameter_info_t c1_parameters[1] =
    {
        { .is_ellipsis = 0, .type_info = c1, .nonadjusted_type_info = NULL }
    };
    parameter_info_t c2_parameters[1] =
    {
        { .is_ellipsis = 0, .type_info = c2, .nonadjusted_type_info = NULL }
    };

    type_t* faked_primary_type_1 = get_new_function_type(get_void_type(), 
            c1_parameters, 1, REF_QUALIFIER_NONE);

    template_parameter_list_t* template_parameters = 
        duplicate_template_argument_list(
                template_specialized_type_get_template_parameters(get_actual_class_type(c1)
                    ));
    // Remove arguments from template parameters
    int i;
    for (i = 0; i < template_parameters->num_parameters; i++)
    {
        template_parameters->arguments[i] = NULL;
    }

    type_t* faked_template_type_1 = get_new_template_type(template_parameters,
            faked_primary_type_1,
            "faked_template_name",
            decl_context,
            locus);

    type_t* faked_type_1 = named_type_get_symbol(
        template_type_get_primary_type(faked_template_type_1)
        )->type_information;

    type_t* faked_type_2 = get_new_function_type(get_void_type(), 
            c2_parameters, 1, REF_QUALIFIER_NONE);

    char result = is_less_or_equal_specialized_template_function_common_(faked_type_1, faked_type_2, 
            named_type_get_symbol(c1)->decl_context, 
            deduced_template_arguments, 
            /* explicit_template_parameters */ NULL,
            locus,
            /* is_conversion */ 0);

    free_temporary_template_type(faked_template_type_1);
    return result;
}

static char is_less_or_equal_specialized_template_conversion_function(
        type_t* f1, type_t* f2, 
        decl_context_t decl_context, 
        template_parameter_list_t** deduced_template_arguments,
        const locus_t* locus)
{
    DEBUG_CODE()
    {
        fprintf(stderr, "TYPEORDER: Computing whether one function is less or equal specialized than the other\n");
    }
    ERROR_CONDITION(!is_function_type(f1) || !is_function_type(f2), "functions types are not", 0);

    int num_arguments = function_type_get_num_parameters(f2);
    if (function_type_get_has_ellipsis(f2))
        num_arguments--;

    int num_parameters = function_type_get_num_parameters(f1);
    if (function_type_get_has_ellipsis(f1))
        num_parameters--;

    if (num_arguments != num_parameters)
    {
        DEBUG_CODE()
        {
            fprintf(stderr, "TYPEORDER: It is not less or equal specialized template function because number arguments != number parameters\n");
        }
        return 0;
    }

    type_t* arguments[1];
    type_t* parameters[1];
    type_t* original_parameters[1];

    num_arguments = 1;
    arguments[0] = function_type_get_parameter_type_num(f2, 0);

    num_parameters = 1;
    original_parameters[0] =
        parameters[0] =
        function_type_get_parameter_type_num(f1, 0);

    // Try to deduce types of template type F1 using F2

    template_parameter_list_t* type_template_parameters = 
        template_specialized_type_get_template_parameters(f1);
    template_parameter_list_t* template_parameters = 
        template_specialized_type_get_template_arguments(f1);

    if (!deduce_template_arguments_common(
                template_parameters, type_template_parameters,
                arguments, num_arguments,
                parameters, num_parameters,
                original_parameters,
                decl_context,
                deduced_template_arguments,
                locus,
                /* explicit_template_parameters */ NULL,
                /* is_function_call */ 0))
    {
        DEBUG_CODE()
        {
            fprintf(stderr, "TYPEORDER: No deduction was possible\n");
        }
        return 0;
    }

    // Now check that the updated types match exactly
    decl_context_t updated_context = decl_context;
    updated_context.template_parameters = *deduced_template_arguments;

    {
        type_t* original_type = function_type_get_parameter_type_num(f1, 0);

        type_t* updated_type = update_type(original_type, 
                updated_context,
                locus);

        // Check the soundness of the updated type
        if (updated_type == NULL)
        {
            DEBUG_CODE()
            {
                fprintf(stderr, "TYPEORDER: The deduced type was not constructible\n");
                fprintf(stderr, "TYPEORDER: It is not less or equal specialized template function because type deduction failed\n");
            }
            return 0;
        }

        type_t* argument_type = function_type_get_parameter_type_num(f2, 0);

        if (!equivalent_types(updated_type, argument_type))
        {
            DEBUG_CODE()
            {
                fprintf(stderr, "TYPEORDER: It is not less or equal specialized template function "
                        "because type of parameter (%s) and type of argument (%s) do not match\n",
                        print_declarator(updated_type),
                        print_declarator(argument_type));
            }
            return 0;
        }
    }

    DEBUG_CODE()
    {
        fprintf(stderr, "TYPEORDER: It IS less or equal specialized\n");
    }

    return 1;
}


char is_less_or_equal_specialized_template_function(type_t* f1, type_t* f2,
        decl_context_t decl_context, template_parameter_list_t** deduction_set,
        template_parameter_list_t* explicit_template_parameters,
        const locus_t* locus, char is_conversion)
{
    return is_less_or_equal_specialized_template_function_common_(
            f1, f2, decl_context, deduction_set, 
            explicit_template_parameters, 
            locus, is_conversion);
}
