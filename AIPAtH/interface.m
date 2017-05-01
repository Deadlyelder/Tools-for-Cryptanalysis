/* takes a bit sequence and return a padded bit sequence */
function pad(msg)
    return [GF(2)|];
end function;

/* computes the hash of a msg as a bitstring */
function hash(msg)
    return [GF(2)|];
end function;

/* returns the quadratic system of equations representing the hash function */
/* the two others sequences are the message variables and the hash variables */
/* optional : set the target message length */
/* optional : non-fixed IV */
function equations(:fixedIV:=true)
    return [],[],[];
end function;
