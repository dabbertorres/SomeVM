CXX    := $(GLOBAL_CXX)
CFLAGS := $(GLOBAL_CFLAGS) -shared -fPIC

RLS_FLAGS := $(GLOBAL_RLS_FLAGS)
DBG_FLAGS := $(GLOBAL_DBG_FLAGS)

OUT       := libSomeVM.so
BUILD_DIR := build

SRC := $(wildcard *.cpp)
OBJ := $(SRC:%.cpp=%.o)
DEP := $(OBJ:%.o=%.d)

.PHONY: release debug

release: $(OUT_DIR)/release/$(OUT)
debug: $(OUT_DIR)/debug/$(OUT)

$(OUT_DIR)/release/$(OUT): $(addprefix $(BUILD_DIR)/release/,$(OBJ))
	$(CXX) $(CFLAGS) $(RLS_FLAGS) $^ -o $(OUT_DIR)/release/$(OUT)

$(OUT_DIR)/debug/$(OUT): $(addprefix $(BUILD_DIR)/debug/,$(OBJ))
	$(CXX) $(CFLAGS) $(DBG_FLAGS) $^ -o $(OUT_DIR)/debug/$(OUT)

# generate dependencies
-include $(BUILD_DIR)$(DEP)

# object files rule
# each object file should have a corresponding .cpp file
# "-MMD" tells the compiler to generate a dependency file for the input
$(BUILD_DIR)/release/%.o : %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) $(RLS_FLAGS) -MMD -c $^ -o $@

$(BUILD_DIR)/debug/%.o : %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) $(DBG_FLAGS) -MMD -c $^ -o $@

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

