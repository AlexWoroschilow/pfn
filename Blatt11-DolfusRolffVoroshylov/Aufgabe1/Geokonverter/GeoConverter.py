from Geokonverter.EncoderManager import EncoderManager


class GeoConverter():
    def __init__(self, encoders):
        self.__manager = EncoderManager(encoders)


    def process(self, files, batch):

        for file in files:
            format_parser = self.__manager.get_decoder_for(file)
            if format_parser:
                yield format_parser.decode(file)

            if not batch:
                break

    def encode(self, entity, format):
        encoder = self.__manager.get_encoder_for(format)
        return encoder.encode(entity)

