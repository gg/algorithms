'''Conway's Game of Life algorithm.
For reference: http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
'''

import itertools
import random
import time


def transition(state):
    '''Returns the next state after transitioning the `state` of the current
    generation of cells as follows:
    - Any live cell with fewer than two live neighbours dies, as if caused by
      under-population.
    - Any live cell with two or three live neighbours lives on to the next
      generation.
    - Any live cell with more than three live neighbours dies, as if by
      overcrowding.
    - Any dead cell with exactly three live neighbours becomes a live cell, as
      if by reproduction.
    '''
    next_state = set()
    cells_to_consider = state | set(itertools.chain(*map(neighbors, state)))
    for cell in cells_to_consider:
        num_neighbors = sum(neighbor in state for neighbor in neighbors(cell))
        if num_neighbors == 3 or (num_neighbors == 2 and cell in state):
            next_state.add(cell)
    return next_state


def neighbors(cell):
    i, j = cell
    return ((i - 1, j - 1), (i - 1, j), (i - 1, j + 1), (i, j - 1), (i, j + 1),
            (i + 1, j - 1), (i + 1, j), (i + 1, j + 1))


def state_to_string(state, alive_char='o', dead_char=' '):
    rows = sorted({i for i, j in state})

    max_col = max(j for i, j in state)
    min_col = min(j for i, j in state)

    top_left_point = str((rows[0], min_col))

    s = top_left_point + ' ' + ('-' * (max_col - len(top_left_point))) + '\n'

    prev_row = rows[0]
    for row in rows:
        s += '\n' * (abs(row) - abs(prev_row) - 1)
        prev_row = row
        prev_col = 0
        for col in sorted(j for i, j in state if i == row):
            s += dead_char * (abs(col) - abs(prev_col) - 1)
            s += alive_char
            prev_col = col
        s += '\n' 

    bot_right_point = str((rows[-1], max_col))
    s += ('-' * (max_col - len(bot_right_point))) + ' ' + bot_right_point
    s += '\n\n'
    return s


def generate_random_state(rows, cols, chance_of_cell_being_alive=0.1):
    state = set()
    for i, j in itertools.product(range(rows), range(cols)):
        if random.randrange(100) < (100 * chance_of_cell_being_alive):
            state.add((i, j))
    return state


if __name__ == '__main__':
    import time
    try:
        rows = 20
        cols = 30
        state = generate_random_state(rows, cols, 0.5)
        prev_state = None
        while state != prev_state:
            print(state_to_string(state))
            prev_state = state
            state = transition(state)
            time.sleep(0.5)
    except KeyboardInterrupt:
        pass
