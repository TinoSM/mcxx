! <testinfo>
! test_generator=config/mercurium-ompss
! test_compile_fail=yes
! </testinfo>
PROGRAM P
    IMPLICIT NONE
    INTEGER :: I

    !$OMP DO FIRSTPRIVATE(I)
    DO I=1, 100
    ENDDO
END PROGRAM P
