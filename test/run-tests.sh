function do_test() {
	local ral_compiler=$1
	local test_dir=$2
	local current_base=$3

	$ral_compiler $test_dir/$current_base.kum >/tmp/ral/$current_base.ll 2>/dev/null
	clang -Xlinker --export-dynamic -x ir /tmp/ral/$current_base.ll -o /tmp/ral/$current_base.elf 2>/dev/null
	/tmp/ral/$current_base.elf >/tmp/ral/$current_base.txt 2>&1
	diff $test_dir/expected/$current_base.txt /tmp/ral/$current_base.txt
	local result=$? 
	if [ $result -eq 0 ]
	then
	  echo "OK $current_base"
	else
	  echo "ERROR $current_base"
	fi
	return $result
}

function do_test_1() {
	local ral_compiler=$1
	local test_dir=$2
	local current_base=$3
	local cin_data=$4

	$ral_compiler $test_dir/$current_base.kum >/tmp/ral/$current_base.ll 2>/dev/null
	clang -Xlinker --export-dynamic -x ir /tmp/ral/$current_base.ll -o /tmp/ral/$current_base.elf 2>/dev/null
	echo $4 | /tmp/ral/$current_base.elf >/tmp/ral/$current_base.txt 2>&1
	diff $test_dir/expected/$current_base.txt /tmp/ral/$current_base.txt
	local result=$? 
	if [ $result -eq 0 ]
	then
	  echo "OK $current_base"
	else
	  echo "ERROR $current_base"
	fi
	return $result
}

function do_error() {
	local ral_compiler=$1
	local test_dir=$2
	local current_base=$3
	local error_string=$4

	$ral_compiler $test_dir/$current_base.kum 2>&1 1>/tmp/ral/$current_base.ll | grep "$error_string" >/dev/null
	local result=$? 
	if [ $result -eq 0 ]
	then
	  echo "OK $current_base"
	else
	  echo "ERROR $current_base"
	fi
	return $result
}

rm -rf /tmp/ral
mkdir -p /tmp/ral
tot=0
do_test $1 $2 "1-empty"
let tot+=$?
do_test $1 $2 "2-2+2"
let tot+=$?
do_test $1 $2 "28-rec-nod"
let tot+=$?
do_test $1 $2 "3-a+b"
let tot+=$?
do_test $1 $2 "4-a+b"
let tot+=$?
do_test_1 $1 $2 "dev-test" "125 234"
let tot+=$?
do_error $1 $2 "f01-bad-alg-arg-type" "promote"
let tot+=$?
do_error $1 $2 "f02-bad-comparison-string" "promote"
let tot+=$?
do_error $1 $2 "f03-bad-logical-type" "promote"
let tot+=$?
do_test $1 $2 "dev-type"
exit $tot
