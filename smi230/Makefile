TARGET ?= hal_test

SRCS := \
	sensord/axis_remap.c\
	sensord/sensord_hwcntl.cpp\
	sensord/sensord_hwcntl_implement.cpp\
	sensord/util_misc.c\
	sensord/sensord_pltf.c\
	sensord/sensord_cfg.cpp\
	sensord/sensord_algo.cpp\
	sensord/sensord.cpp\
	sensord/boschsimple_list.cpp\
	hal/sensors.cpp\
	hal/BoschSensor.cpp

OBJS := $(addsuffix .o,$(basename $(SRCS)))
#DEPS := $(addsuffix .d,$(basename $(SRCS)))

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

HAL_TEST ?= true

ifeq ($(HAL_TEST), true)
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -DPLTF_LINUX_ENABLED -DTEST_APP_ACTIVE
LDFLAGS ?= -lpthread -lstdc++
else
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -DPLTF_LINUX_ENABLED
LDFLAGS ?= -lpthread -lstdc++ -shared
endif

hal_test: $(SRCS)
	arm-linux-gnueabihf-gcc $(CPPFLAGS) -lpthread -lstdc++ $(SRCS) $(LDFLAGS) -o hal_test

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS) hal_test.d
