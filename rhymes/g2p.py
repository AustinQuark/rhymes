import g2p_en

g2p = g2p_en.G2p()

def convert_to_phonemes(word):
    return g2p(word)
