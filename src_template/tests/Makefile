
# This file is a part of __PROGRAM_NAME__ __PROGRAM_VERSION__
#
# This file makes automatic tests for the program parts.
#
# __PROGRAM_COPYRIGHT__ __PROGRAM_AUTHOR__ __PROGRAM_AUTHOR_EMAIL__
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


# Build section

CC = gcc
CFLAGS = -ansi -pedantic -Wall
LDFLAGS = -lcunit

BUILD_DIR = build

TARGET_BIGNUMBER = test_bignumber
SRC_BIGNUMBER_TEST = test_bignumber.c
SRC_BIGNUMBER_SOURCES = ../bignumber.c
TARGET_FILE_OPERATION = test_file_operation
SRC_FILE_OPERATION_TEST = test_file_operation.c
SRC_FILE_OPERATION_SOURCES = ../file_operation.c ../bignumber.c ../file_offset.c ../crc32.c
TARGET_CHAIN = test_chain
SRC_CHAIN_TEST = test_chain.c
SRC_CHAIN_SOURCES = ../chain.c ../bignumber.c ../file_offset.c \
                    ../bindir.c ../binfile.c ../datetime.c \
                    ../file_operation.c ../crc32.c ../node.c \
	            ../binfield.c ../endian.c ../jumper.c
TARGET_BINFILE = test_binfile
SRC_BINFILE_TEST = test_binfile.c
SRC_BINFILE_SOURCES = ../binfile.c ../binfield.c ../bignumber.c ../endian.c
TARGET_CRC32 = test_crc32
SRC_CRC32_TEST = test_crc32.c
SRC_CRC32_SOURCES = ../crc32.c
TARGET_DATETIME = test_datetime
SRC_DATETIME_TEST = test_datetime.c
SRC_DATETIME_SOURCES = ../datetime.c
TARGET_BINDIR = test_bindir
SRC_BINDIR_TEST = test_bindir.c
SRC_BINDIR_SOURCES = ../bindir.c ../binfield.c ../endian.c


# Commands

all: prepare_build_directory bignumber file_operation chain binfile crc32 datetime bindir

prepare_build_directory:
	@[ -d $(BUILD_DIR) ] $&& rm -rf $(BUILD_DIR)
	@mkdir $(BUILD_DIR)

bignumber:
	@$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_BIGNUMBER_SOURCES) $(SRC_BIGNUMBER_TEST) -o $(BUILD_DIR)/$(TARGET_BIGNUMBER)
	@echo "$(TARGET_BIGNUMBER) has built"

file_operation:
	@$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_FILE_OPERATION_SOURCES) $(SRC_FILE_OPERATION_TEST) -o $(BUILD_DIR)/$(TARGET_FILE_OPERATION)
	@echo "$(TARGET_FILE_OPERATION) has built"

chain:
	@$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_CHAIN_SOURCES) $(SRC_CHAIN_TEST) -o $(BUILD_DIR)/$(TARGET_CHAIN)
	@echo "$(TARGET_CHAIN) has built"

binfile:
	@$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_BINFILE_SOURCES) $(SRC_BINFILE_TEST) -o $(BUILD_DIR)/$(TARGET_BINFILE)
	@echo "$(TARGET_BINFILE) has built"

crc32:
	@$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_CRC32_SOURCES) $(SRC_CRC32_TEST) -o $(BUILD_DIR)/$(TARGET_CRC32)
	@echo "$(TARGET_CRC32) has built"

datetime:
	@$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_DATETIME_SOURCES) $(SRC_DATETIME_TEST) -o $(BUILD_DIR)/$(TARGET_DATETIME)
	@echo "$(TARGET_DATETIME) has built"

bindir:
	@$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_BINDIR_SOURCES) $(SRC_BINDIR_TEST) -o $(BUILD_DIR)/$(TARGET_BINDIR)
	@echo "$(TARGET_BINDIR) has built"

help:
	@echo "usage: make [ clean ]" 1>&2

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned $(BUILD_DIR)/$(TARGET_BIGNUMBER)"
	@echo "Cleaned $(BUILD_DIR)/$(TARGET_FILE_OPERATION)"
	@echo "Cleaned $(BUILD_DIR)/$(TARGET_CHAIN)"
	@echo "Cleaned $(BUILD_DIR)/$(TARGET_BINFILE)"
	@echo "Cleaned $(BUILD_DIR)/$(TARGET_CRC32)"
	@echo "Cleaned $(BUILD_DIR)/$(TARGET_DATETIME)"
	@echo "Cleaned $(BUILD_DIR)/$(TARGET_BINDIR)"

.PHONY: all prepare_build_directory bignumber file_operation chain binfile crc32 datetime bindir help clean
