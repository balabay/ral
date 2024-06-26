function do_test() {
	local ral_compiler=$1
	local test_dir=$2
	local current_base=$3
	local stdral_dir=`dirname $ral_compiler`

	$ral_compiler $test_dir/$current_base.kum >/tmp/ral/$current_base.ll 2>/dev/null
	clang -x ir /tmp/ral/$current_base.ll -L$stdral_dir/libstdral -l stdral -l m -o /tmp/ral/$current_base.elf 2>/dev/null
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
	local stdral_dir=`dirname $ral_compiler`

	$ral_compiler $test_dir/$current_base.kum >/tmp/ral/$current_base.ll 2>/dev/null
	clang -x ir /tmp/ral/$current_base.ll -L$stdral_dir/libstdral -l stdral -l m -o /tmp/ral/$current_base.elf 2>/dev/null
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

function do_test_with_input_and_suffix() {
	local ral_compiler=$1
	local test_dir=$2
	local current_base=$3
	local cin_data=$4
	local suffix=$5
	local stdral_dir=`dirname $ral_compiler`

	$ral_compiler $test_dir/$current_base.kum >/tmp/ral/$current_base.ll 2>/dev/null
	clang -x ir /tmp/ral/$current_base.ll -L$stdral_dir/libstdral -l stdral -l m -o /tmp/ral/$current_base.elf 2>/dev/null
	echo $4 | /tmp/ral/$current_base.elf >/tmp/ral/$current_base.txt 2>&1
	diff $test_dir/expected/$current_base.$5.txt /tmp/ral/$current_base.txt
	local result=$? 
	if [ $result -eq 0 ]
	then
	  echo "OK $current_base $suffix"
	else
	  echo "ERROR $current_base $suffix"
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
do_test $1 $2 "3-a+b"
let tot+=$?
do_test $1 $2 "4-a+b"
let tot+=$?
do_test $1 $2 "5-div-mod"
let tot+=$?
do_test $1 $2 "6-format"
let tot+=$?
# no 7 in KUMIR
do_test_1 $1 $2 "8-if" "5 7"
let tot+=$?
do_test_1 $1 $2 "9-if" "5 7"
let tot+=$?
# no 10 in KUMIR
do_test $1 $2 "11-switch"
let tot+=$?
do_test_1 $1 $2 "12-switch" "-5e-3"
let tot+=$?
do_test_with_input_and_suffix $1 $2 "12-switch" "0" "0"
let tot+=$?
do_test_with_input_and_suffix $1 $2 "12-switch" "+12.55" "1"
let tot+=$?
do_test $1 $2 "13-loopN"
let tot+=$?
do_test $1 $2 "14-while"
let tot+=$?
do_test_1 $1 $2 "15-repeat" "-1 0 2"
let tot+=$?
do_test $1 $2 "16-for"
let tot+=$?
do_test $1 $2 "17-downto"
let tot+=$?
do_test_1 $1 $2 "17-downto-fixed" "5"
let tot+=$?
do_test $1 $2 "18-prime"
let tot+=$?
do_test $1 $2 "19-proc-err"
let tot+=$?
do_test $1 $2 "20-proc-bin"
let tot+=$?
do_test $1 $2 "21-swap"
let tot+=$?
do_test $1 $2 "22-func-sumdig"
let tot+=$?
do_test $1 $2 "23-func-prime"
let tot+=$?
do_test_1 $1 $2 "24-func-prime" "5 7 12"
let tot+=$?
# code is modified due to space handling in the function names
# I do not want to spend time on this - just add ;
do_test $1 $2 "25-rec-hanoi"
let tot+=$?
do_test $1 $2 "26-rec-bin"
let tot+=$?
do_test $1 $2 "27-rec-sumdig"
let tot+=$?
do_test $1 $2 "28-rec-nod"
let tot+=$?
do_test $1 $2 "29-rec-fact"
let tot+=$?
do_test_1 $1 $2 "dev-loop" "20"
let tot+=$?
do_test_1 $1 $2 "dev-test" "125 234"
let tot+=$?
do_test $1 $2 "dev-type"
let tot+=$?
do_test $1 $2 "dev-stdral"
let tot+=$?
do_error $1 $2 "f01-bad-alg-arg-type" "promote"
let tot+=$?
do_error $1 $2 "f02-bad-comparison-string" "promote"
let tot+=$?
do_error $1 $2 "f03-bad-logical-type" "promote"
let tot+=$?
do_error $1 $2 "f04-alg-void-return" "cannot assign"
let tot+=$?
do_error $1 $2 "f05-alg-return-type-mismatch" "promote"
let tot+=$?
do_error $1 $2 "f06-if-type-mismatch" "promote"
let tot+=$?
do_error $1 $2 "f07-for-type-mismatch" "Loop index"
let tot+=$?
do_error $1 $2 "f08-for-no-index" "Not a variable"
let tot+=$?
exit $tot
