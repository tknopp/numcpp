#include <numcpp/core.h>
#include <numcpp/base.h>
#include <numcpp/expressions.h>
#include <numcpp/io.h>

using namespace numcpp;

int main(int argc, char* argv[])
{
  std::string filenameSF = argv[1];
  std::string filenameSNR = argv[2];
  std::string filenameMeasurements = argv[3];
  std::string filenameReco = argv[4];


  auto SNR = fromfile<double>(filenameSNR);

  std::vector<size_t> shape = {64, 40};

  auto K = SNR.size();
  auto N = prod(shape);
  int nrUsedRows = 100;

  auto tmp = argsort(SNR);
  flipud_(tmp);
  auto rowIndices = tmp(S{0,nrUsedRows});
  sort_(rowIndices);


  return 0;
}

/*
def denseRecoMPI(filenameIn, filenameMatrix, filenameSNR, filenameOut, shape, nrRecChan, nrUsedRows,
                 nrRows=-1, lambd=0, timeDomain=True, dtypeMeas=np.int16, dtypeMatrix=np.cdouble, **kwargs):
  Perform image reconstruction of MPI data


  pyreco.computeSNRMatrixRows(filenameMatrix, filenameSNR, shape, nrRows = nrRows)

  SNR = np.fromfile(filenameSNR, dtype=np.double)

  K = len(SNR)
  N = np.prod(shape)

  rowIndices = np.sort(np.flipud(np.argsort(SNR))[0:nrUsedRows])
  A = pyreco.loadMatrix(filenameMatrix, dtype=dtypeMatrix, nrCols=N, rowIndices=rowIndices)

  if lambd > 0:
    trace = pyreco.calculateTraceOfNormalMatrix(A)
    lambd *= trace / N

  fdIn = open(filenameIn,'rb')
  fdOut = open(filenameOut,'wb')

  sizeBytes = pyreco.fileSize(fdIn)
  itemsize = np.dtype(dtypeMeas).itemsize
  size = sizeBytes / itemsize
  M = ((K / nrRecChan)-1)*2
  nrFrames = size / M / nrRecChan

  for l in xrange(nrFrames):
    print("denseRecoMPI: frame=%d, " % l )

    b = np.double(np.fromfile(fdIn, dtype=dtypeMeas, count=M*nrRecChan))

    if timeDomain:
      bHat = np.zeros(K, dtype=dtypeMatrix)
      for r in xrange(nrRecChan):
        bHat[r*K/nrRecChan:(r+1)*K/nrRecChan] = np.fft.rfft(b[r*M:(r+1)*M])
    else:
      bHat = b

    bHat = bHat[rowIndices].transpose()

    x = pyreco.kaczmarz(A, bHat, lambd=lambd, **kwargs)

    x.tofile(fdOut)

  fdIn.close()
  fdOut.close()
*/
