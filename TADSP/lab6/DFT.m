function [X_k] = DFT(x_n)

N = length(x_n); 
X_k = zeros(1, N); 
for k = 0:N-1
    for n = 0:N-1
        X_k(k+1) = X_k(k+1) + x_n(n+1) * (cos(2*pi*n*k/N)-1i*sin(2*pi*n*k/N));
    end
end
end
