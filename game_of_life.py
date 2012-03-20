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


def run_random_game():
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


def pairwise(seq):
    """Returns a pairwise generator over a non-empty sequence `seq`, wrapping
    the last item with the first.

    >>> for i, j in pairwise([1, 2, 3, 4, 5]):
    ...     print((i, j))
    ...
    (1, 2)
    (2, 3)
    (3, 4)
    (4, 5)
    
    Credit to Martin v. Löwis: http://stackoverflow.com/a/1257446
    """
    i = iter(seq)
    first = prev = item = next(i)
    for item in i:
        yield prev, item
        prev = item


def test_patterns():
    # Still lifes
    block = set([(1, 1), (1, 2), (2, 1), (2, 2)])
    beehive = set([(1, 2), (1, 3), (2, 1), (2, 4), (3, 2), (3, 3)])
    loaf = set([(1, 2), (1, 3), (2, 1), (2, 4), (3, 2), (3, 4), (4, 3)])
    boat = set([(1, 1), (1, 2), (2, 1), (2, 3), (3, 2)])
    for state in (block, beehive, loaf, boat):
        assert(transition(state) == state)

    # Oscillators
    blinker = [set([(1, 0), (1, 1), (1, 2)]),
               set([(2, 1), (1, 1), (0, 1)])]
    toad = [set([(2, 2), (2, 3), (2, 4), (3, 1), (3, 2), (3, 3)]),
            set([(1, 3), (2, 1), (2, 4), (3, 1), (3, 4), (4, 2)])]
    beacon = [set([(1, 1), (1, 2), (2, 1), (2, 2), (3, 3), (3, 4), (4, 3),
                   (4, 4)]),
              set([(1, 1), (1, 2), (2, 1), (3, 4), (4, 3), (4, 4)])]
    pulsar = [set([(1, 5), (1, 11), (2, 5), (2, 11), (3, 5), (3, 6), (3, 10),
                   (3, 11), (5, 1), (5, 2), (5, 3), (5, 6), (5, 7), (5, 9),
                   (5, 10), (5, 13), (5, 14), (5, 15), (6, 3), (6, 5), (6, 7),
                   (6, 9), (6, 11), (6, 13), (7, 5), (7, 6), (7, 10), (7, 11),
                   (9, 5), (9, 6), (9, 10), (9, 11), (10, 3), (10, 5), (10, 7),
                   (10, 9), (10, 11), (10, 13), (11, 1), (11, 2), (11, 3),
                   (11, 6), (11, 7), (11, 9), (11, 10), (11, 13), (11, 14),
                   (11, 15), (13, 5), (13, 6), (13, 10), (13, 11), (14, 5),
                   (14, 11), (15, 5), (15, 11)]),
              set([(2, 4), (2, 5), (2, 11), (2, 12), (3, 5), (3, 6), (3, 10),
                   (3, 11), (4, 2), (4, 5), (4, 7), (4, 9), (4, 11), (4, 14),
                   (5, 2), (5, 3), (5, 4), (5, 6), (5, 7), (5, 9), (5, 10),
                   (5, 12), (5, 13), (5, 14), (6, 3), (6, 5), (6, 7), (6, 9),
                   (6, 11), (6, 13), (7, 4), (7, 5), (7, 6), (7, 10), (7, 11),
                   (7, 12), (9, 4), (9, 5), (9, 6), (9, 10), (9, 11), (9, 12),
                   (10, 3), (10, 5), (10, 7), (10, 9), (10, 11), (10, 13),
                   (11, 2), (11, 3), (11, 4), (11, 6), (11, 7), (11, 9),
                   (11, 10), (11, 12), (11, 13), (11, 14), (12, 2), (12, 5),
                   (12, 7), (12, 9), (12, 11), (12, 14), (13, 5), (13, 6),
                   (13, 10), (13, 11), (14, 4), (14, 5), (14, 11), (14, 12)]),
              set([(2, 4), (2, 5), (2, 6), (2, 10), (2, 11), (2, 12), (4, 2),
                   (4, 7), (4, 9), (4, 14), (5, 2), (5, 7), (5, 9), (5, 14),
                   (6, 2), (6, 7), (6, 9), (6, 14), (7, 4), (7, 5), (7, 6),
                   (7, 10), (7, 11), (7, 12), (9, 4), (9, 5), (9, 6), (9, 10),
                   (9, 11), (9, 12), (10, 2), (10, 7), (10, 9), (10, 14),
                   (11, 2), (11, 7), (11, 9), (11, 14), (12, 2), (12, 7),
                   (12, 9), (12, 14), (14, 4), (14, 5), (14, 6), (14, 10),
                   (14, 11), (14, 12)])]
    for states in (blinker, toad, beacon, pulsar):
        for state, next_state in pairwise(states):
            assert(transition(state) == next_state)
        # An oscillator transitions from its last state to its first state.
        assert(transition(states[-1]) == states[0])

    # Spaceships
    glider = [set([(1, 1), (2, 2), (2, 3), (3, 1), (3, 2)]),
              set([(1, 2), (2, 3), (3, 1), (3, 2), (3, 3)]),
              set([(2, 1), (2, 3), (3, 2), (3, 3), (4, 2)]),
              set([(2, 3), (3, 1), (3, 3), (4, 2), (4, 3)]),
              set([(2, 2), (3, 3), (3, 4), (4, 2), (4, 3)])]
    for state, next_state in pairwise(glider):
        assert(transition(state) == next_state)


if __name__ == '__main__':
    test_patterns()
    run_random_game()
