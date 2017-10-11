
# Introduction

Porting of my Python version of Unum v2: https://github.com/eruffaldi/pyunum2

# Usage

Use them as templates with variadic arguments expressing the lattice:
	
	unum<T,1,a,b,c,d,...>

	unum<uint8_t,1,2,4,5>

The type T is the older of the values (e.g. uint8_t for an 8-bit)

# Status

Times and Summations are needed, either as slow op or as table