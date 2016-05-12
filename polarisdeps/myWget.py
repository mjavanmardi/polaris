import requests
import argparse
import os.path
import sys
import zipfile
import tarfile

def download_file(url, outputdir):
	local_filename = outputdir + url.split('/')[-1]
	# NOTE the stream=True parameter
	# python -mrequests.certs
	r = requests.get(url, stream=True, verify=True)
	file_size = int(r.headers['content-length'])
	print "Downloading: %s Bytes: %s" % (local_filename, file_size)
	with open(local_filename, 'wb') as f:
		file_size_dl = 0
		for chunk in r.iter_content(chunk_size=1024): 
			if chunk: # filter out keep-alive new chunks
				file_size_dl += len(chunk)
				f.write(chunk)
				# status = r"%10d  [%3.2f%%]" % (file_size_dl, file_size_dl * 100. / file_size)
				# status = status + chr(8)*(len(status)+1)
				# print status,
	return local_filename
	
parser = argparse.ArgumentParser(description='Process some files.')
parser.add_argument("-u", "--url", type=str, required=True, help='URL of file to download')
parser.add_argument("-n", "--name", type=str, required=True, help='NAME of the downloaded file')
parser.add_argument("-e", "--extracted_dir", type=str, required=True, help='DIRECTORY of the extracted file')
parser.add_argument("-o", "--output_dir", type=str, default="./", help='output directory (default: current directory)')
parser.add_argument("-r", "--rename", help='RENAME the dowloaded file using the --name argument', action="store_true")
args=parser.parse_args()

url = args.url
outdir = args.output_dir
abspath = os.path.abspath(outdir)
abspath = os.path.join(abspath, '')
file_dl = os.path.join(abspath, args.name)
extractdir = os.path.join(abspath, args.extracted_dir)
print 'absolute output path="%s"' % abspath
# create the download directory if it doesn't exist
if not os.path.exists(abspath):
	os.makedirs(abspath)
if not os.path.exists(abspath):
	print 'output path "%s" does not exist' % abspath
	sys.exit()

# does the downloaded file already exist?
#if not os.path.exists(file_dl):
	file_dl = download_file(url, abspath)
	if args.rename:
		#build new name
		path = os.path.dirname(file_dl)
		newname = os.path.join(path, args.name)
		os.rename(file_dl, newname)
		file_dl = newname
#else:
#	print 'File already downloaded'
	
# does the extracted directory already exist?
#if not os.path.exists(extractdir):
	if file_dl.lower().endswith('.zip'):
		with zipfile.ZipFile(file_dl, "r") as z:
			print 'Extracting %s to %s' % (file_dl, abspath)
			z.extractall(abspath)
	else:
		with tarfile.open(file_dl, 'r:*') as z:
			print 'Extracting %s to %s' % (file_dl, abspath)
			z.extractall(abspath)
#else:
#	print 'File already extracted'
