Program TestProgram {
    int x;
    int arr[10];
    float y;

    x = 5
    y = 3

    if (x < 10) {
        x = x + 1
    }

    if (x > 5) {
        x = x - 1
    } else {
        x = 0
    }

    while (x < 100) {
        x = x * 2
    }

    arr[0] = x
    y = arr[0] + y

    x = (x + 1) * (y - 2)
}.
