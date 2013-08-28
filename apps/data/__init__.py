#import subprocess
def classFactory(iface):
    from polaris import Polaris
    return Polaris(iface)