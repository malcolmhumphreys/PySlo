
class test_rsl2_big(
    float Ks = .5;
    float Kd = .52;
    point far = 1;
    vector up = (0, 1, 0);
    normal foo[3] = 0; 
    string str = "dfdf";
    string astr[2] = {"testing 123", "sdfsdf"};
    matrix identity = 1;
    matrix zero[5] = 0;
    float ff[4] = { 0, 1, 2, 3 };
    color cf[10] = 1;
    float frf[] = { 1,2,3 };
    ) {
    
    #pragma annotation "author" "Malcolm Humphreys"
    #pragma annotation "version" "1.0"
    #pragma annotation "Ks comment" "Specular intensity"
    #pragma annotation "Ks range" "[0..10]"
    #pragma annotation "has_shadow type" "toggle"
    
    public void displacement(output point P; output normal N) {
    }
    
    public void surface(output color Ci, Oi) {
    }
    
    public void test_test(output color foo, fumm) {
    }
    
}

