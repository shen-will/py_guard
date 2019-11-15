

#encode/decode python code,bestter modify it yourself

def encode(ins:bytearray):
    '''
    :param ins:  bytearray bytecode
    :return: ins bytearray modifyed
    '''

    for i in range(len(ins)):
        ins[i] = ins[i]+2

    return ins


def decode(ins: bytearray):
    '''
    :param ins:  bytearray bytecode
    :return: ins bytearray modifyed
    '''

    for i in range(len(ins)):
        ins[i] = ins[i] - 2

    return ins
