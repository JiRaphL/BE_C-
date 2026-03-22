CXX := g++
CXXFLAGS := -std=gnu++17 -Wall -Wextra -O2
CPPFLAGS := -Iinclude

ifeq ($(OS),Windows_NT)
	LDLIBS_PLATFORM := -lgdi32 -luser32
	EXE_EXT := .exe
else
	LDLIBS_PLATFORM := -lX11 -lpthread
	EXE_EXT :=
endif

APP_TARGET := aquarium$(EXE_EXT)
TEST_TARGET := aquarium_tests$(EXE_EXT)

APP_SRCS := \
	src/main.cpp \
	src/Aquarium.cpp \
	src/Bestiole.cpp \
	src/SimulationConfig.cpp \
	src/app/Simulation.cpp \
	src/app/StatisticsCollector.cpp \
	src/BestioleFactory.cpp \
	src/Milieu.cpp \
	src/render/CImgRenderer.cpp \
	src/managers/CollisionManager.cpp \
	src/managers/LifecycleManager.cpp \
	src/sensors/EyeSensor.cpp \
	src/sensors/EarSensor.cpp \
	src/accessories/FinAccessory.cpp \
	src/accessories/ShellAccessory.cpp \
	src/accessories/CamouflageAccessory.cpp \
	src/behavior/GregariousBehavior.cpp \
	src/behavior/FearfulBehavior.cpp \
	src/behavior/KamikazeBehavior.cpp \
	src/behavior/CautiousBehavior.cpp

CORE_SRCS := \
	src/Aquarium.cpp \
	src/Bestiole.cpp \
	src/SimulationConfig.cpp \
	src/app/Simulation.cpp \
	src/app/StatisticsCollector.cpp \
	src/BestioleFactory.cpp \
	src/Milieu.cpp \
	src/managers/CollisionManager.cpp \
	src/managers/LifecycleManager.cpp \
	src/sensors/EyeSensor.cpp \
	src/sensors/EarSensor.cpp \
	src/accessories/FinAccessory.cpp \
	src/accessories/ShellAccessory.cpp \
	src/accessories/CamouflageAccessory.cpp \
	src/behavior/GregariousBehavior.cpp \
	src/behavior/FearfulBehavior.cpp \
	src/behavior/KamikazeBehavior.cpp \
	src/behavior/CautiousBehavior.cpp

TEST_SRCS := \
	tests/framework/TestFramework.cpp \
	tests/test_main.cpp \
	tests/unit/BehaviorTests.cpp \
	tests/unit/SensorTests.cpp \
	tests/unit/AccessoryTests.cpp \
	tests/unit/CollisionLifecycleTests.cpp \
	tests/unit/RuntimeManagerTests.cpp \
	tests/integration/SimulationIntegrationTests.cpp \
	tests/integration/StatisticsIntegrationTests.cpp \
	$(CORE_SRCS)

APP_OBJS := $(APP_SRCS:.cpp=.o)
TEST_OBJS := $(TEST_SRCS:.cpp=.o)

.PHONY: all app tests test clean

all: app

app: $(APP_TARGET)

tests: $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(APP_TARGET): $(APP_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS_PLATFORM)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS_PLATFORM)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -Itests -c $< -o $@

clean:
	rm -f $(APP_TARGET) $(TEST_TARGET) $(APP_OBJS) $(TEST_OBJS)
