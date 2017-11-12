import serial

ser = serial.Serial('/dev/tty.wchusbserial14130', 9600, timeout=10)

def clearmem():
    ser.write(b'c')
    ser.flush()

    conf = ser.readline()
    if conf:
        print('Clearing finished\n')
        return

    print('Unknown error, arduino did not respond.')

def downloadcsv():
    ser.write(b'r')
    ser.flush()
    total_entries = int(ser.readline())

    print('Total entries in current run : {}'.format(total_entries))

    all_entries = []

    for curr_idx in range(total_entries):
        a, b = int(ser.readline()), int(ser.readline())
        print('Initial press time/Duration of press : {}ms/{}ms'.format(a, b))
        all_entries.append((a, b))

    if not all_entries:
        return

    file_name = input('File to save to [default name: output]? : ')
    if not file_name:
        file_name = 'output'

    file_name += '.csv'

    with open(file_name, 'w') as f:
        f.write('Start press time (ms),Press duration (ms)\n')
        for entry in all_entries:
            f.write('{},{}\n'.format(*entry))
    
    print('File written to {}\n'.format(file_name))



def helpcommands():
    print('\nPossible commands are')
    print('Display this help message: h')
    print('Clear memory (may take up to ten seconds): c')
    print('Download values: d')
    print('Quit: q')
    print('-------')

POSSIBLE_INPUTS = {
    'c': clearmem,
    'd': downloadcsv,
    'h': helpcommands,
    'q': exit
}

print('Waiting for Arduino')
curr_entry = ser.readline()
if not curr_entry:
    print('Arduino did not respond. Make sure it\' connected and you\'re on the right port!')
    exit()
print('Arduino responded with : {} !'.format(curr_entry))

helpcommands()

while True:
    curr_input = input('Command input: ')
    if not curr_input:
        continue

    if curr_input in POSSIBLE_INPUTS:
        POSSIBLE_INPUTS[curr_input]()
        continue

    print('Invalid command.')


