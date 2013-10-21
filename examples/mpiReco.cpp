#include <numcpp/core.h>
#include <numcpp/base.h>
#include <numcpp/expressions.h>
#include <numcpp/io.h>
#include <numcpp/solvers.h>
#include <numcpp/fft.h>
#include <numcpp/graphics.h>

using namespace numcpp;

int main(int argc, char* argv[])
{
  std::string filenameMatrix = argv[1];
  std::string filenameSNR = argv[2];
  std::string filenameMeasurements = argv[3];
  std::string filenameReco = argv[4];
  double lambda = 5e-3;
  std::vector<size_t> shape = {68, 40};

  auto N = 68*40;//prod(shape);
  int nrUsedRows = 1400;
  int nrRecChan = 2;



  auto SNR = fromfile<double>(filenameSNR);

  auto K = SNR.size();

  auto tmp = argsort(SNR);
  reverse_(tmp);
  auto rowIndices = tmp(S{0,nrUsedRows});
  sort_(rowIndices);
  auto A = loadMatrix<cdouble>(filenameMatrix, N, rowIndices);

  if(lambda > 0)
  {
    auto energy = rowEnergy(A);
    double trace = pow(norm(energy),2);
    lambda *= trace / N;
  }



  auto sizeBytes = filesize(filenameMeasurements);
  int itemsize = sizeof(int16_t);
  auto size = sizeBytes / itemsize;
  auto M = ((K / nrRecChan)-1)*2;
  auto nrFrames = size / M / nrRecChan;

  std::ifstream fdIn(filenameMeasurements, std::ios::in|std::ios::binary);
  std::ofstream fdOut(filenameReco, std::ios::out|std::ios::binary|std::ios::trunc);

  for(int l=0; l<nrFrames; l++)
  {
    std::cout << "denseRecoMPI: frame=" << l << std::endl;

    Array<cdouble> bHatFull(K);
    for(int r=0; r<nrRecChan; r++)
    {
      Array<double> inducedSignal = fromfile<int16_t>(fdIn, M);
      bHatFull(S{r*K/nrRecChan,(r+1)*K/nrRecChan}) = rfft(inducedSignal);
    }

    Array<cdouble> bHat(nrUsedRows);
    for(int k=0; k<nrUsedRows; k++)
      bHat(k) = bHat(rowIndices(k));

    auto x = kaczmarz(A, bHat, 10, lambda);

    Array<double> xR = abs(reshape(x,40,68));
    export_image(xR , "reco.png");

    tofile(x,fdOut);

  }

  fdIn.close();
  fdOut.close();

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
