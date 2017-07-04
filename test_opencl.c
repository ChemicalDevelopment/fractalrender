
// test file

int main(int argc, char *argv[]) {
    int x;
    int platform_id;
    clGetPlatformIDs(1, &platform_id, &x);
    return 0;
}
