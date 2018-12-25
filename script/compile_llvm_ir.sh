if [ $# -lt 1 ]; then
	echo "bad input count: $#, usage: bash compile.sh filename.cpp"
	exit 1
fi

filename=$(echo "$1" | cut -f 1 -d '.')

# compile IR
clang -emit-llvm -S $1

# compile bc
clang -emit-llvm -c $1 -o ${filename}.bc

# assemble
# llc compile LLVM IR further to assembly
llc ${filename}.ll
# or: clang -S add.cpp -o add.s 

# compile object file
clang -fmodules -c $1 -o ${filename}.o
# or: clang -c add.cpp

# link to module
clang $1 -o ${filename}.bin

