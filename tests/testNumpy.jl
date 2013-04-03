
function test()
N=70
M=10

x=zeros(Complex128, N,N,N)
for i=0:M-1
  x += 1
end

for i=0:(N-1)
  for j=0:(N-1)
    for k=0:(N-1)
      x[i+1,j+1,k+1] += i+j+k
    end
  end
end

y = reshape(x,N*N*N)

#for i=0:M-1
#  x += i*exp(2*pi*im*i* reshape(linspace(0,1,N*N*N),N,N,N))
#end

for i=0:M-1
  for j=0:N*N*N-1
    y[j+1] += i*exp(2*pi*im*i* (j/(N*N*N-1)))
  end
end

for i=0:M-1
  x[1:2:N,1:2:N,1:2:N] += i
end

end

tic()

for i=1:10
  test()
end

toc()

