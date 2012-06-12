MODULE ISO_C_BINDING, INTRINSIC
    INTEGER, PARAMETER :: C_INT = MERCURIUM_C_INT
    INTEGER, PARAMETER :: C_SHORT = MERCURIUM_C_SHORT
    INTEGER, PARAMETER :: C_LONG = MERCURIUM_C_LONG
    INTEGER, PARAMETER :: C_LONG_LONG = MERCURIUM_C_LONG_LONG
    INTEGER, PARAMETER :: C_SIGNED_CHAR = MERCURIUM_C_SIGNED_CHAR
    INTEGER, PARAMETER :: C_SIZE_T = MERCURIUM_C_SIZE_T

    INTEGER, PARAMETER :: C_INT8_T = MERCURIUM_C_INT8_T
    INTEGER, PARAMETER :: C_INT16_T = MERCURIUM_C_INT16_T
    INTEGER, PARAMETER :: C_INT32_T = MERCURIUM_C_INT32_T
    INTEGER, PARAMETER :: C_INT64_T = MERCURIUM_C_INT64_T
    !  INTEGER, PARAMETER :: C_INT128_T = 16

    ! FIXME - These may be not realistic as Mercurium does not know anything
    ! about "least" integer types per environment (at the moment)
    INTEGER, PARAMETER :: C_INT_LEAST8_T = MERCURIUM_C_INT_LEAST8_T
    INTEGER, PARAMETER :: C_INT_LEAST16_T = MERCURIUM_C_INT_LEAST16_T
    INTEGER, PARAMETER :: C_INT_LEAST32_T = MERCURIUM_C_INT_LEAST32_T
    INTEGER, PARAMETER :: C_INT_LEAST64_T = MERCURIUM_C_INT_LEAST64_T
    ! INTEGER :: C_INT_LEAST128_T = 16

    ! FIXME - These may be not realistic as Mercurium does not know anything
    ! about "fast" integer types per environment (at the moment)
    INTEGER, PARAMETER :: C_INT_FAST8_T = MERCURIUM_C_INT_FAST8_T
    INTEGER, PARAMETER :: C_INT_FAST16_T = MERCURIUM_C_INT_FAST16_T
    INTEGER, PARAMETER :: C_INT_FAST32_T = MERCURIUM_C_INT_FAST32_T
    INTEGER, PARAMETER :: C_INT_FAST64_T = MERCURIUM_C_INT_FAST64_T
    ! INTEGER :: C_INT_FAST128_T

    INTEGER, PARAMETER :: C_INTMAX_T = 8
    INTEGER, PARAMETER :: C_INTPTR_T = MERCURIUM_C_INTPTR_T
    INTEGER, PARAMETER :: C_FLOAT = MERCURIUM_C_FLOAT
    INTEGER, PARAMETER :: C_DOUBLE = MERCURIUM_C_DOUBLE
    INTEGER, PARAMETER :: C_LONG_DOUBLE = MERCURIUM_C_LONG_DOUBLE
    INTEGER, PARAMETER :: C_FLOAT_COMPLEX = MERCURIUM_C_FLOAT_COMPLEX
    INTEGER, PARAMETER :: C_DOUBLE_COMPLEX = MERCURIUM_C_DOUBLE_COMPLEX
    INTEGER, PARAMETER :: C_LONG_DOUBLE_COMPLEX = MERCURIUM_C_LONG_DOUBLE_COMPLEX
    INTEGER, PARAMETER :: C_BOOL = MERCURIUM_C_BOOL
    INTEGER, PARAMETER :: C_CHAR = MERCURIUM_C_CHAR
    CHARACTER(KIND=C_CHAR), PARAMETER :: C_NULL_CHAR = CHAR(0)
    CHARACTER(KIND=C_CHAR), PARAMETER :: C_ALERT = CHAR(7)
    CHARACTER(KIND=C_CHAR), PARAMETER :: C_BACKSPACE = CHAR(8)
    CHARACTER(KIND=C_CHAR), PARAMETER :: C_FORM_FEED = CHAR(12)
    CHARACTER(KIND=C_CHAR), PARAMETER :: C_NEW_LINE = CHAR(10)
    CHARACTER(KIND=C_CHAR), PARAMETER :: C_CARRIAGE_RETURN = CHAR(13)
    CHARACTER(KIND=C_CHAR), PARAMETER :: C_HORIZONTAL_TAB = CHAR(9)
    CHARACTER(KIND=C_CHAR), PARAMETER :: C_VERTICAL_TAB = CHAR(11)

    TYPE C_PTR
        INTEGER(KIND=MERCURIUM_C_PTR) :: PTR
    END TYPE C_PTR
    TYPE C_FUNPTR
        INTEGER(KIND=MERCURIUM_C_FUNPTR) :: FUNPTR
    END TYPE C_FUNPTR

    TYPE(C_PTR), PARAMETER :: C_NULL_PTR = C_PTR(INT(0, KIND=MERCURIUM_C_PTR))
    TYPE(C_FUNPTR), PARAMETER :: C_NULL_FUNPTR = C_FUNPTR(INT(0, KIND=MERCURIUM_C_FUNPTR))

    INTERFACE C_ASSOCIATED
        FUNCTION C_ASSOCIATED_PTR(c_ptr_1, c_ptr_2)
            IMPLICIT NONE
            IMPORT C_PTR
            LOGICAL :: C_ASSOCIATED_PTR
            TYPE(C_PTR), INTENT(IN) :: C_PTR_1, C_PTR_2
            OPTIONAL :: C_PTR_2
        END FUNCTION C_ASSOCIATED_PTR

        FUNCTION C_ASSOCIATED_FUNPTR(c_ptr_1, c_ptr_2)
            IMPLICIT NONE
            IMPORT C_FUNPTR
            LOGICAL :: C_ASSOCIATED_FUNPTR
            TYPE(C_FUNPTR), INTENT(IN) :: C_PTR_1, C_PTR_2
            OPTIONAL :: C_PTR_2
        END FUNCTION C_ASSOCIATED_FUNPTR
    END INTERFACE C_ASSOCIATED

INCLUDE "machine_generated_iso_c_binding.f90"
END MODULE ISO_C_BINDING
