SUPPRESSING_WARNINGS := -Wall -Wshadow -Wmissing-prototypes -Wextra -Wno-unused-function -std=c99 \
	-Wpointer-arith -Wuninitialized -Wmaybe-uninitialized -Wtype-limits -Wreturn-type -Wclobbered \
	-Wunreachable-code -Wformat-security -Wsizeof-pointer-memaccess -Wdouble-promotion \
	-Wdeprecated -Wempty-body -Wstack-usage=2048 -Wpointer-arith \
	-fno-strict-aliasing -Wno-unused-label\
	-Wno-unused-value -Wno-unused-variable -Wno-undef -Wno-error=cpp \
	-Wno-error=strict-prototypes -Wno-unused-parameter -Wno-cast-qual -Wno-ignored-qualifiers \
	-Wno-error=pedantic -Wno-switch-default -Wno-sign-compare -Wno-error=missing-prototypes \
	-Wno-missing-field-initializers -Wno-format-nonliteral 

ifeq ($(CROSS_COMPILE), )
WARNINGS	:= $(SUPPRESSING_WARNINGS) -Wshift-negative-value -Wno-discarded-qualifiers -Wno-int-conversion
else
WARNINGS	:= $(SUPPRESSING_WARNINGS)
endif
