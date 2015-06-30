class EncoderManager():
    def __init__(self, encoders):
        self.__encoders = encoders

    def get_decoder_for(self, file):
        for parser in self.__encoders:
            if parser.can_decode(file):
                return parser
        return None


    def get_encoder_for(self, format):
        for converter in self.__encoders:
            if converter.can_encode(format):
                return converter
        return None