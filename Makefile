# Advent of Code 2020 - Makefile
CCFLAGS = "-g"
CC = "gcc"
SRC_D_SUFFIX = "day"


all: day1 day2 day3 day4 day5 day7 day8
day1:
	@echo CC -o ${SRC_D_SUFFIX}1/day1.out
	@${CC} -o ${SRC_D_SUFFIX}1/day1.out ${SRC_D_SUFFIX}1/day1.c ${CCFLAGS}
day2:
	@echo CC -o ${SRC_D_SUFFIX}2/day2.out
	@${CC} -o ${SRC_D_SUFFIX}2/day2.out ${SRC_D_SUFFIX}2/day2.c ${CCFLAGS}
day3:
	@echo CC -o ${SRC_D_SUFFIX}3/day3.out
	@${CC} -o ${SRC_D_SUFFIX}3/day3.out ${SRC_D_SUFFIX}3/day3.c ${CCFLAGS}
day4:
	@echo CC -o ${SRC_D_SUFFIX}4/day4.out
	@${CC} -o ${SRC_D_SUFFIX}4/day4.out ${SRC_D_SUFFIX}4/day4.c ${CCFLAGS}
day5:
	@echo CC -o ${SRC_D_SUFFIX}5/day5.out
	@${CC} -o ${SRC_D_SUFFIX}5/day5.out ${SRC_D_SUFFIX}5/day5.c ${CCFLAGS}
day6:
	@echo CC -o ${SRC_D_SUFFIX}6/day6.out
	@${CC} -o ${SRC_D_SUFFIX}6/day6.out ${SRC_D_SUFFIX}6/day6.c ${CCFLAGS}
day7:
	@echo CC -o ${SRC_D_SUFFIX}7/day7.out
	@${CC} -o ${SRC_D_SUFFIX}7/day7.out ${SRC_D_SUFFIX}7/day7.c ${CCFLAGS}
day8:
	@echo CC -o ${SRC_D_SUFFIX}8/day8.out
	@${CC} -o ${SRC_D_SUFFIX}8/day8.out ${SRC_D_SUFFIX}8/day8.c ${CCFLAGS}


.PHONY: all day1 day2 day3 day4 day5 day7 day8
