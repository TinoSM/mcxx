#include "tl-omptransform.hpp"

namespace TL
{
    void OpenMPTransform::get_data_explicit_attributes(
            Scope function_scope,
            OpenMP::Directive directive,
            Statement construct_body,
            ObjectList<IdExpression>& shared_references,
            ObjectList<IdExpression>& private_references,
            ObjectList<IdExpression>& firstprivate_references,
            ObjectList<IdExpression>& lastprivate_references,
            ObjectList<OpenMP::ReductionIdExpression>& reduction_references,
            ObjectList<IdExpression>& copyin_references,
            ObjectList<IdExpression>& copyprivate_references)
    {
        // Get references in shared clause
        OpenMP::Clause shared_clause = directive.shared_clause();
        shared_references = shared_clause.id_expressions();

        // Get references in private_clause
        OpenMP::Clause private_clause = directive.private_clause();
        private_references = private_clause.id_expressions();

        // Get references in firstprivate clause
        OpenMP::Clause firstprivate_clause = directive.firstprivate_clause();
        firstprivate_references = firstprivate_clause.id_expressions();

        // Get references in lastprivate clause
        OpenMP::Clause lastprivate_clause = directive.lastprivate_clause();
        lastprivate_references = lastprivate_clause.id_expressions();

        // Get references in reduction clause
        OpenMP::ReductionClause reduction_clause = directive.reduction_clause();
        reduction_references = reduction_clause.id_expressions();

        // Get references in copyin
        OpenMP::Clause copyin_clause = directive.copyin_clause();
        copyin_references = copyin_clause.id_expressions();

        // Get references in copyprivate
        OpenMP::Clause copyprivate_clause = directive.copyprivate_clause();
        copyprivate_references = copyprivate_clause.id_expressions();
    }


    void OpenMPTransform::get_data_attributes(
            Scope function_scope,
            OpenMP::Directive directive,
            Statement construct_body,
            ObjectList<IdExpression>& shared_references,
            ObjectList<IdExpression>& private_references,
            ObjectList<IdExpression>& firstprivate_references,
            ObjectList<IdExpression>& lastprivate_references,
            ObjectList<OpenMP::ReductionIdExpression>& reduction_references,
            ObjectList<IdExpression>& copyin_references,
            ObjectList<IdExpression>& copyprivate_references)
    {
        get_data_explicit_attributes(
                function_scope,
                directive,
                construct_body,
                shared_references,
                private_references,
                firstprivate_references,
                lastprivate_references,
                reduction_references,
                copyin_references,
                copyprivate_references);

        enum
        {
            PK_DATA_INVALID = 0,
            PK_DATA_SHARED, 
            PK_DATA_PRIVATE,
            PK_DATA_NONE,
        } default_data_sharing = PK_DATA_INVALID;

        OpenMP::DefaultClause default_clause = directive.default_clause();

        if (!default_clause.is_defined())
        {
            // By default it is shared
            default_data_sharing = PK_DATA_SHARED;
        }
        else if (default_clause.is_none())
        {
            default_data_sharing = PK_DATA_NONE;
        }
        else if (default_clause.is_shared())
        {
            default_data_sharing = PK_DATA_SHARED;
        }
        // An extension that we consider sensible
        else if (default_clause.is_custom("private"))
        {
            default_data_sharing = PK_DATA_PRIVATE;
        }
        else
        {
            std::cerr << "Warning: Unknown default clause '" 
                << default_clause.prettyprint() << "' at " << default_clause.get_ast().get_locus() << ". "
                << "Assuming 'default(shared)'."
                << std::endl;
            default_data_sharing = PK_DATA_SHARED;
        }

        // Get every non local reference: this is, not defined in the
        // construct itself, but visible at the point where the
        // construct is defined
        ObjectList<IdExpression> non_local_references = construct_body.non_local_symbol_occurrences(Statement::ONLY_VARIABLES);
        ObjectList<Symbol> non_local_symbols = non_local_references.map(functor(&IdExpression::get_symbol));

        // Filter shareds, privates, firstprivate, lastprivate or
        // reduction that are useless
        ObjectList<IdExpression> unreferenced;
        // Add to unreferenced symbols that appear in shared_references but not in non_local_references
        unreferenced.append(shared_references.filter(not_in_set(non_local_references, functor(&IdExpression::get_symbol))));
        // shared_references now only contains references that appear in non_local_references
        shared_references = shared_references.filter(in_set(non_local_references, functor(&IdExpression::get_symbol)));

        // Add to unreferenced symbols that appear in private_references but not in non_local_references
        unreferenced.append(private_references.filter(not_in_set(non_local_references, functor(&IdExpression::get_symbol))));
        // private_references now only contains references that appear in non_local_references
        private_references = private_references.filter(in_set(non_local_references, functor(&IdExpression::get_symbol)));

        // Add to unreferenced symbols that appear in lastprivate_references but not in non_local_references
        unreferenced.append(firstprivate_references.filter(not_in_set(non_local_references, functor(&IdExpression::get_symbol))));
        // firstprivate_references now only contains references that appear in non_local_references
        firstprivate_references = firstprivate_references.filter(in_set(non_local_references, functor(&IdExpression::get_symbol)));

        // Add to unreferenced symbols that appear in lastprivate_references but not in non_local_references
        unreferenced.append(lastprivate_references.filter(not_in_set(non_local_references, functor(&IdExpression::get_symbol))));
        // lastprivate_references now only contains references that appear in non_local_references
        lastprivate_references = lastprivate_references.filter(in_set(non_local_references, functor(&IdExpression::get_symbol)));

        // Add to unreferenced symbols that appear in copyin_references but not in non_local_references
        unreferenced.append(copyin_references.filter(not_in_set(non_local_references, functor(&IdExpression::get_symbol))));
        // copyin_references now only contains references that appear in non_local_references
        copyin_references = copyin_references.filter(in_set(non_local_references, functor(&IdExpression::get_symbol)));

        // Add to unreferenced symbols that appear in copyprivate_references but not in non_local_references
        unreferenced.append(copyprivate_references.filter(not_in_set(non_local_references, functor(&IdExpression::get_symbol))));
        // copyprivate_references now only contains references that appear in non_local_references
        copyprivate_references = copyprivate_references.filter(in_set(non_local_references, functor(&IdExpression::get_symbol)));

        // Add to unreferenced symbols that appear in reduction_references but not in non_local_references
        unreferenced.append(
                reduction_references.filter(not_in_set(non_local_symbols, 
                        functor(&OpenMP::ReductionIdExpression::get_symbol)))
                .map(functor(&OpenMP::ReductionIdExpression::get_id_expression))
                );
        // reduction_references now only contains references that appear in non_local_references
        reduction_references = reduction_references.filter(in_set(non_local_symbols, 
                    functor(&OpenMP::ReductionIdExpression::get_symbol)));

        // Will give a warning for every unreferenced element
        unreferenced.map(functor(&OpenMPTransform::warn_unreferenced_data, *this));

        // If a symbol appears into shared_references, private_references, firstprivate_references, lastprivate_references
        // or copyin_references, copyprivate_references, remove it from non_local_references
        non_local_references = non_local_references.filter(not_in_set(shared_references, functor(&IdExpression::get_symbol)));
        non_local_references = non_local_references.filter(not_in_set(private_references, functor(&IdExpression::get_symbol)));
        non_local_references = non_local_references.filter(not_in_set(firstprivate_references, functor(&IdExpression::get_symbol)));
        non_local_references = non_local_references.filter(not_in_set(lastprivate_references, functor(&IdExpression::get_symbol)));
        non_local_references = non_local_references.filter(not_in_set(copyin_references, functor(&IdExpression::get_symbol)));
        non_local_references = non_local_references.filter(not_in_set(copyprivate_references, functor(&IdExpression::get_symbol)));

        // Get every id-expression related to the ReductionIdExpression list
        ObjectList<IdExpression> reduction_id_expressions = 
            reduction_references.map(functor(&OpenMP::ReductionIdExpression::get_id_expression));
        // and remove it from non_local_references
        non_local_references = non_local_references.filter(not_in_set(reduction_id_expressions, functor(&IdExpression::get_symbol)));

        switch ((int)default_data_sharing)
        {
            case PK_DATA_NONE :
                {
                    non_local_references.map(functor(&OpenMPTransform::warn_no_data_sharing, *this));
                    /* Fall through shared */
                }
            case PK_DATA_SHARED :
                {
                    shared_references.insert(non_local_references, functor(&IdExpression::get_symbol));
                    break;
                }
            case PK_DATA_PRIVATE :
                {
                    private_references.insert(non_local_references, functor(&IdExpression::get_symbol));
                    break;
                }
            case PK_DATA_INVALID :
            default:
                {
                    break;
                }
        }
    }

    ReplaceIdExpression OpenMPTransform::set_replacements(FunctionDefinition function_definition,
            OpenMP::Directive directive,
            Statement construct_body,
            ObjectList<IdExpression>& shared_references,
            ObjectList<IdExpression>& private_references,
            ObjectList<IdExpression>& firstprivate_references,
            ObjectList<IdExpression>& lastprivate_references,
            ObjectList<OpenMP::ReductionIdExpression>& reduction_references,
            ObjectList<OpenMP::ReductionIdExpression>& inner_reduction_references,
            ObjectList<IdExpression>& copyin_references,
            ObjectList<IdExpression>& copyprivate_references,
            ObjectList<ParameterInfo>& parameter_info,
            bool share_always /* = false */)
    {
        Symbol function_symbol = function_definition.get_function_name().get_symbol();
        Scope function_scope = function_definition.get_scope();
        ReplaceIdExpression result;

        // SHARED references
        for (ObjectList<IdExpression>::iterator it = shared_references.begin();
                it != shared_references.end();
                it++)
        {
            // We ignore unqualified/qualified references that are function accessible
            // or unqualified references that are data members of the same class
            // of this function because they can be accessed magically
            if (!share_always 
                    && is_function_accessible(*it, function_definition)
                    && !is_unqualified_member_symbol(*it, function_definition))
                continue;

            Symbol symbol = it->get_symbol();
            if (!is_unqualified_member_symbol(*it, function_definition))
            {
                Type type = symbol.get_type();
                Type pointer_type = type.get_pointer_to();

                ParameterInfo parameter(it->mangle_id_expression(), 
                        "&" + it->prettyprint(), *it, pointer_type, ParameterInfo::BY_POINTER);
                parameter_info.append(parameter);
                result.add_replacement(symbol, "(*" + it->mangle_id_expression() + ")", 
                        it->get_ast(), it->get_scope_link());
            }
            else
            {
                // Only if this function is a nonstatic one we need _this access
                if (is_nonstatic_member_function(function_definition))
                {
                    result.add_replacement(symbol, "_this->" + it->prettyprint(), 
                            it->get_ast(), it->get_scope_link());
                }
            }
        }

        // PRIVATE references
        for (ObjectList<IdExpression>::iterator it = private_references.begin();
                it != private_references.end();
                it++)
        {
            Symbol symbol = it->get_symbol();
            Type type = symbol.get_type();

            result.add_replacement(symbol, "p_" + it->mangle_id_expression(),
                    it->get_ast(), it->get_scope_link());
        }

        // FIRSTPRIVATE references
        for (ObjectList<IdExpression>::iterator it = firstprivate_references.begin();
                it != firstprivate_references.end();
                it++)
        {
            Symbol symbol = it->get_symbol();
            Type type = symbol.get_type();

            Type pointer_type = type.get_pointer_to();

            ParameterInfo parameter("flp_" + it->mangle_id_expression(), 
                    "&" + it->prettyprint(), 
                    *it, pointer_type, ParameterInfo::BY_POINTER);
            parameter_info.append(parameter);

            result.add_replacement(symbol, "p_" + it->mangle_id_expression(),
                    it->get_ast(), it->get_scope_link());
        }

        // LASTPRIVATE references
        for (ObjectList<IdExpression>::iterator it = lastprivate_references.begin();
                it != lastprivate_references.end();
                it++)
        {
            Symbol symbol = it->get_symbol();
            Type type = symbol.get_type();

            Type pointer_type = type.get_pointer_to();

            ParameterInfo parameter("flp_" + it->mangle_id_expression(), 
                    "&" + it->prettyprint(),
                    *it, pointer_type, ParameterInfo::BY_POINTER);
            parameter_info.append(parameter);

            result.add_replacement(symbol, "p_" + it->mangle_id_expression(),
                    it->get_ast(), it->get_scope_link());
        }

        // REDUCTION references
        for (ObjectList<OpenMP::ReductionIdExpression>::iterator it = reduction_references.begin();
                it != reduction_references.end();
                it++)
        {
            IdExpression id_expr = it->get_id_expression();
            Symbol symbol = id_expr.get_symbol();
            Type type = symbol.get_type();

            Type pointer_type = type.get_pointer_to();

            ParameterInfo parameter("rdv_" + id_expr.mangle_id_expression(), 
                    "rdv_" + id_expr.mangle_id_expression(),
                    id_expr, pointer_type, ParameterInfo::BY_POINTER);
            parameter_info.append(parameter);

            result.add_replacement(symbol, "rdp_" + id_expr.mangle_id_expression(),
                    id_expr.get_ast(), id_expr.get_scope_link());
        }

        // Inner REDUCTION references (those coming from lexical enclosed DO's inner to this PARALLEL)
        for (ObjectList<OpenMP::ReductionIdExpression>::iterator it = inner_reduction_references.begin();
                it != inner_reduction_references.end();
                it++)
        {
            IdExpression id_expr = it->get_id_expression();
            Symbol symbol = id_expr.get_symbol();
            Type type = symbol.get_type();

            Type pointer_type = type.get_pointer_to();

            ParameterInfo reduction_vector_parameter("rdv_" + id_expr.mangle_id_expression(), 
                    "rdv_" + id_expr.mangle_id_expression(),
                    id_expr, pointer_type, ParameterInfo::BY_POINTER);

            parameter_info.append(reduction_vector_parameter);

            ParameterInfo parameter(id_expr.mangle_id_expression(), 
                    "&" + id_expr.prettyprint(),
                    id_expr, pointer_type, ParameterInfo::BY_POINTER);

            result.add_replacement(symbol, "(*" + id_expr.mangle_id_expression() + ")",
                    id_expr.get_ast(), id_expr.get_scope_link());
        }

        // COPYIN references
        for (ObjectList<IdExpression>::iterator it = copyin_references.begin();
                it != copyin_references.end();
                it++)
        {
            Symbol symbol = it->get_symbol();
            Type type = symbol.get_type();

            Type pointer_type = type.get_pointer_to();

            ParameterInfo parameter("cin_" + it->mangle_id_expression(), 
                    "&" + it->prettyprint(),
                    *it, type, ParameterInfo::BY_POINTER);
            parameter_info.append(parameter);
        }

        // COPYPRIVATE references
        for (ObjectList<IdExpression>::iterator it = copyprivate_references.begin();
                it != copyprivate_references.end();
                it++)
        {
            Symbol symbol = it->get_symbol();
            Type type = symbol.get_type();

            Type pointer_type = type.get_pointer_to();

            ParameterInfo parameter("cout_" + it->mangle_id_expression(), 
                    "&" + it->prettyprint(),
                    *it, pointer_type, ParameterInfo::BY_POINTER);
            parameter_info.append(parameter);
        }

        if (is_nonstatic_member_function(function_definition))
        {
            // Calls to nonstatic member functions within the body of the construct
            // of a nonstatic member function
            ObjectList<IdExpression> function_references = 
                construct_body.non_local_symbol_occurrences(Statement::ONLY_FUNCTIONS);
            for (ObjectList<IdExpression>::iterator it = function_references.begin();
                    it != function_references.end();
                    it++)
            {
                if (is_unqualified_member_symbol(*it, function_definition))
                {
                    Symbol symbol = it->get_symbol();
                    result.add_replacement(symbol, "_this->" + it->prettyprint(),
                            it->get_ast(), it->get_scope_link());
                }
            }
        }

        return result;
    }
}
