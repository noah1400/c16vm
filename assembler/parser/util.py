

def asType(type):
    def inner(value):
        return {'type': type, 'value': value}
    return inner