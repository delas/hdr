CPP = g++
C_FLAGS =
L_FLAGS = -lfann
SRC_DIR = src

all: train test convert bulk-test own-test

train: $(SRC_DIR)/train.cpp
	$(CPP) $(SRC_DIR)/train.cpp $(C_FLAGS) $(L_FLAGS) -o train

test: $(SRC_DIR)/test.cpp
	$(CPP) $(SRC_DIR)/test.cpp $(C_FLAGS) $(L_FLAGS) -o test

bulk-test: $(SRC_DIR)/bulk-test.cpp
	$(CPP) $(SRC_DIR)/bulk-test.cpp $(C_FLAGS) $(L_FLAGS) -o bulk-test

own-test: $(SRC_DIR)/own-test.cpp
	$(CPP) $(SRC_DIR)/own-test.cpp $(C_FLAGS) $(L_FLAGS) -lIL -lILU -o own-test

convert: $(SRC_DIR)/convert.cpp
	$(CPP) $(SRC_DIR)/convert.cpp $(C_FLAGS) -o convert

clean:
	rm -rf test train convert bulk-test
