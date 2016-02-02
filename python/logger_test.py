import logging
import logging.config

logging.config.fileConfig("logger.conf")
logger = logging.getLogger("example01")



logger.debug('This is debug message')
logger.info('This is info message')
logger.warning('This is warning message')


logging.config.fileConfig('log.conf')
ws_debug = logging.getLogger('debugLogger').debug
ws_notice = logging.getLogger('noticeLogger').info
ws_warning = logging.getLogger('warningLogger').warning
ws_error = logging.getLogger('errorLogger').error
ws_fatal = logging.getLogger('fatalLogger').critical

print "hello fuck world"

ws_debug('this is custom debug message')
ws_notice('this is custom info message')
ws_warning('this is custom warning message')