
function performance01()
  N=1000000
  M=10

  tic()

  for l=1:10
    x=zeros(Complex128, N)
    for i=0:M-1
      x += 1
    end

    for i=0:(N-1)
      x[i+1] += i
    end

    for i=0:M-1
      x += i*exp(2*pi*im*i*linspace(0,1,N))
    end

    for i=0:M-1
      x += i
    end
  end

  toc()
end

performance01()



