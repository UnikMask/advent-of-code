# Advent of Code 2020 - Makefile
CCFLAGS = "-g"
CC = "clang"
SRC_D_SUFFIX = "day"


all: day1 day2 day3 day4 day5 day7 day8 day9 day10
day1:
	@echo CC -o ${SRC_D_SUFFIX}01/day01
	@${CC} -o ${SRC_D_SUFFIX}01/day01 ${SRC_D_SUFFIX}01/day1.c ${CCFLAGS}
day2:
	@echo CC -o ${SRC_D_SUFFIX}02/day02
	@${CC} -o ${SRC_D_SUFFIX}02/day02 ${SRC_D_SUFFIX}02/day2.c ${CCFLAGS}
day3:
	@echo CC -o ${SRC_D_SUFFIX}03/day03
	@${CC} -o ${SRC_D_SUFFIX}03/day03 ${SRC_D_SUFFIX}03/day3.c ${CCFLAGS}
day4:
	@echo CC -o ${SRC_D_SUFFIX}04/day04
	@${CC} -o ${SRC_D_SUFFIX}04/day04 ${SRC_D_SUFFIX}04/day4.c ${CCFLAGS}
day5:
	@echo CC -o ${SRC_D_SUFFIX}05/day05
	@${CC} -o ${SRC_D_SUFFIX}05/day05 ${SRC_D_SUFFIX}05/day5.c ${CCFLAGS}
day6:
	@echo CC -o ${SRC_D_SUFFIX}06/day06
	@${CC} -o ${SRC_D_SUFFIX}06/day06 ${SRC_D_SUFFIX}06/day6.c ${CCFLAGS}
day7:
	@echo CC -o ${SRC_D_SUFFIX}07/day07
	@${CC} -o ${SRC_D_SUFFIX}07/day07 ${SRC_D_SUFFIX}07/day7.c ${CCFLAGS}
day8:
	@echo CC -o ${SRC_D_SUFFIX}08/day08
	@${CC} -o ${SRC_D_SUFFIX}08/day08 ${SRC_D_SUFFIX}08/day8.c ${CCFLAGS}
day9:
	@echo CC -o ${SRC_D_SUFFIX}09/day09
	@${CC} -o ${SRC_D_SUFFIX}09/day09 ${SRC_D_SUFFIX}09/day9.c ${CCFLAGS}
day10:
	@echo CC -o ${SRC_D_SUFFIX}10/day10.out
	@${CC} -o ${SRC_D_SUFFIX}10/day10 ${SRC_D_SUFFIX}10/day10.c ${CCFLAGS}
clean:
	@rm day*/day??


.PHONY: all day1 day2 day3 day4 day5 day7 day8 day9 day10
