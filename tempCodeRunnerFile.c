void draw_maze(struct maze_context* maze, int y_pos, int x_pos) {
    int h = maze->h;
    int w = maze->w;
    struct cell* c = maze->c;

    struct cell* m;
    int x_gap = 4;
    int y_gap = 2;

    // Calculate the size of the output buffer
    int rows = h * y_gap + 3;
    int cols = w * x_gap + 5;

    // Allocate the output buffer
    char** output = malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        output[i] = malloc(cols * sizeof(char));
        memset(output[i], ' ', cols - 1);
        output[i][cols - 1] = '\0';
    }

    // Draw the left edges of the cells
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int cc = get_leftwall_code(maze, y, x);
            int row = (y * y_gap) + y_pos;
            int col = (x * x_gap) + x_pos;
            memcpy(&output[row][col], corner[cc], strlen(corner[cc]));
        }
    }

    // Draw the bottom edges of the bottom-most cells
    for (int x = 0; x < w; x++) {
        int cc = get_bottomwall_code(maze, (h - 1), x);
        int row = ((h - 1) * y_gap) + y_pos + 2;
        int col = (x * x_gap) + x_pos;
        memcpy(&output[row][col], corner[cc], strlen(corner[cc]));
    }

    // Draw the right edges of the right-most cells
    for (int y = 0; y < h; y++) {
        int cc = get_rightwall_code(maze, y, (w - 1));
        int row = (y * y_gap) + y_pos;
        int col = ((w - 1) * x_gap) + x_pos + 4;
        memcpy(&output[row][col], corner[cc], strlen(corner[cc]));
    }

    // Draw the bottom-right corner of the maze
    int cc1 = get_brcorner_code(maze);
    int row = ((h - 1) * y_gap) + y_pos + 2;
    int col = ((w - 1) * x_gap) + x_pos + 4;
    memcpy(&output[row][col], corner[cc1], strlen(corner[cc1]));

    // Draw the walls
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            m = &c[(y * w) + x];

            // Top wall
            int row = (y * y_gap) + y_pos;
            int col = (x * x_gap) + x_pos + 1;
            if (m->top_wall) {
                memcpy(&output[row][col], "───", 3);
            }

            // Bottom wall
            row = (y * y_gap) + y_pos + 2;
            col = (x * x_gap) + x_pos + 1;
            if (m->bottom_wall) {
                memcpy(&output[row][col], "───", 3);
            }

            // Left wall
            row = (y * y_gap) + y_pos + 1;
            col = (x * x_gap) + x_pos;
            if (m->left_wall) {
                output[row][col] = '│';
            }

            // Right wall
            col = (x * x_gap) + x_pos + 4;
            if (m->right_wall) {
                output[row][col] = '│';
            }
        }
    }

    // Print the maze
    for (int i = 0; i < rows; i++) {
        printf("%s\n", output[i]);
        free(output[i]);
    }
    free(output);
}