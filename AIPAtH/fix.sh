# fix hash_name round_numbers fixed_bits fix_method
function fix ()
{
    DIR=$1/r$2
    FILE=$(mktemp)
    head -n $3 $DIR/$4.anf >$FILE
    cat $DIR/h.anf >>$FILE
    cat $DIR/F.anf >>$FILE
    echo $FILE
}
