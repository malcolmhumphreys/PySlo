
surface
test_rsl1_big(
float Ks = .5;
float Kd = .52;
point far = 1;
vector up = (0, 1, 0);
//normal foo[3] = 0;  // not supported by aqsis
string str = "dfdf";
string astr[2] = {"testing 123", "sdfsdf"};
matrix identity = 1;
//matrix zero[5] = 0;  // not supported by aqsis
float ff[4] = { 0, 1, 2, 3 };
//color cf[10] = 1; // not supported by aqsis
//float frf[] = { 1,2,3 };  // not supported by aqsis
) {
    Ci = 0.18;
    Oi = 1.0;
    Ci *= Oi;
}