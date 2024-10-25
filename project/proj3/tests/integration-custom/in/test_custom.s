# test i type instruction
# addi test
addi t0, x0, 3
addi t0, x0, -2
# srai test
srai t1, t0, 1
addi t0, x0, 5
srai t1, t0, 2
# andi ori xori test
addi t0, x0, 27
andi t1, t0, 3
ori t1, t0, 4
xori t1, t0, 5
# slli test
slli t1, t0, 3
# srli test
srli t1, t0, 2
addi t0, x0, -7
srli t1, t0, 3
# slti test
slti t1, t0, 3
slti t1, t0, -3
slti t1, t0, -9
addi t0, x0, 9
slti t1, t0, 3
slti t1, t0, 12
