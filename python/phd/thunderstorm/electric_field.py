import numpy as np
from numpy.random import random_sample
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import axes3d



# def firstVersion():
#     # Программа создает распределение заряда и внутриоблачного потенциала с учетом отражения от Земли (нужно скопировать в редактор  MatLab и запустить).
#     # %3D BrownianGaussLandscape formation
#     # %MyBGLand3D
#     # global G ww F
#     N = 5;  # 65
#     n = 5;  # 75
#     R = 1;
#     A = 2;
#     F = np.zeros((N, N, n));
#     ax = np.arange(N);
#     ay = np.arange(N);
#     az = np.arange(n);
#     X, Y, Z = np.meshgrid(ax, ay, az);
#
#     for t in range(1, 6):
#         R = 2 * R
#         M = round((N ** 3) / (R ** 3)) * t  # number of spots
#         A = A / 2;
#         # spot formation
#         s = 0;
#         while s < M:
#             x = round(N * random_sample());
#             y = round(N * random_sample());
#             z = round(n * random_sample());
#             g = (X - x) ** 2 / R ** 2 + (Y - y) ** 2 / R ** 2 + (Z - z) ** 2 / R ** 2;
#             h = 2 * round(random_sample()) - 1;
#             G = h * np.exp(-g);
#             F = F + A * G;
#             s = s + 1;
#     # %+++++++++++++++++
#     ww = F.shape;
#     ax = np.arange(ww[0]);
#     ay = np.arange(ww[1]);
#     az = np.arange(ww[2]);
#     X, Y, Z = np.meshgrid(ax, ay, az);
#     # %+++++++++++++++++
#     L = 0.1;  # scale (km)
#     for i in range(ww[0]):
#         for j in range(ww[1]):
#             for k in range(ww[2]):
#                 RD = L * np.sqrt((X - i) ** 2 + (Y - j) ** 2 + (Z - k) ** 2 + .1);
#                 RR = L * np.sqrt((X - i) ** 2 + (Y - j) ** 2 + (Z + k) ** 2 + .1);
#                 G[i, j, k] = 9 * np.sum(np.sum(np.sum(F * (RD ** (-1) - RR ** (-1)))));  # (MV)
#     P = np.max(G);
#     print(G)
#     # %+++++++++++++++++
#
#     u, v, w = np.gradient(G, L);
#     E_c = np.sqrt(u** 2 + v**2 + w**2);
#     # E = max(max(max(E_c))); dims = [ww(1) ww(2) ww(3)];
#     # [rowsub colsub pagsub] = ind2sub(dims, find(E_c == E));
#     return E_c


def generate_potential(N = 65, n = 75):
    """
    Программа создает распределение заряда и внутриоблачного потенциала с учетом отражения от Земли
    N - number of cell along horizontal side
    n - number of cell along vertical axis
    """
    R = 1
    A = 2
    F = np.zeros((N, N, n))
    ax = np.arange(N)
    ay = np.arange(N)
    az = np.arange(n)
    X, Y, Z = np.meshgrid(ax, ay, az)

    for t in range(1, 6):
        R = 2 * R
        M = round((N ** 3) / (R ** 3)) * t  # number of spots
        A = A / 2
        # spot formation
        s = 0
        x, y, z = map(lambda x: np.random.randint(0, x, size=M), [N, N, n])
        h = 2 * np.round(random_sample(size=M)) - 1
        while s < M:
            F += A * h[s] * np.exp(-(((X - x[s]) ** 2 + (Y - y[s]) ** 2 + (Z - z[s]) ** 2) / R ** 2))
            s += 1
    # %+++++++++++++++++
    # %+++++++++++++++++
    L = 0.1 # scale (km)
    G = np.zeros((N, N, n))
    for i in range(N):
        for j in range(N):
            for k in range(n):
                RD = L * np.sqrt((X - i) ** 2 + (Y - j) ** 2 + (Z - k) ** 2 + .1)
                RR = L * np.sqrt((X - i) ** 2 + (Y - j) ** 2 + (Z + k) ** 2 + .1)
                G[i, j, k] = 9 * np.sum(F * (RD ** (-1) - RR ** (-1)))  # (kV)

    result = {
        'potential' : G,
        'unitPotential' : 'kV',
        'scale' : L*1000,
        'unitScale' : 'm'
    }

    return result


def calculate_electric_field(potential, scale):
    N, N, n = potential.shape
    ax = np.arange(N)
    ay = np.arange(N)
    az = np.arange(n)
    X, Y, Z = np.meshgrid(ax, ay, az)
    Ex, Ey, Ez = np.gradient(potential, scale)
    return Ex, Ey, Ez


def calculate_modula(Ex, Ey, Ez):
    return np.sqrt(Ex ** 2 + Ey ** 2 + Ez ** 2)

def plot_quiver(potential, scale, file_to_save=None):
    N, N, n = potential.shape
    ax = np.arange(N)
    ay = np.arange(N)
    az = np.arange(n)
    X, Y, Z = np.meshgrid(ax, ay, az)
    u, v, w = np.gradient(potential, scale)
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    ax.quiver(X, Y, Z, u, v, w)
    if file_to_save is not None:
        plt.savefig(file_to_save, transparent=True, format="pdf")


# Программа создает распределение заряда и внутриоблачного потенциала с учетом отражения от Земли (нужно скопировать в редактор  MatLab и запустить).
# %3D BrownianGaussLandscape formation
# %MyBGLand3D
# global G ww F
# N = 65; n = 75; R = 1; A = 2; F = zeros(N,N,n);
# ax = 1:N; ay = 1:N; az = 1:n; [X,Y,Z] = meshgrid(ax,ay,az); tic
# for t = 1:6
#     R = 2*R
#     M = round(N^3/(R^3))*t % number of spots
#     A = A/2;
#     %spot formation
#     s=0;
#     while s < M
#         x = round(N*rand); y = round(N*rand); z = round(n*rand);
#         g = (X - x).^2/R^2 + (Y - y).^2/R^2 + (Z - z).^2/R^2;
#         h = 2*round(rand)-1; G = h*exp(-g);
#         F = F + A*G; s = s + 1;
#     end
# end
# %+++++++++++++++++
# ww = size(F); %tic
# ax = 1:ww(1); ay = 1:ww(2); az = 1:ww(3); [X,Y,Z] = meshgrid(ax,ay,az);
# %+++++++++++++++++
# L = .1; % scale (km)
# for i = 1:ww(1)
#     for j = 1:ww(2)
#         for k = 1:ww(3)
#             RD = L*sqrt((X - i).^2 + (Y - j).^2 + (Z - k).^2 + .1);
#             RR = L*sqrt((X - i).^2 + (Y - j).^2 + (Z + k).^2 + .1);
#             G(i,j,k) = 9*sum(sum(sum(F.*(RD.^(-1) - RR.^(-1))))); % (MV)
#         end
#     end
# end
# %toc; P = max(max(max(G)));
# %+++++++++++++++++
# [u,v,w] = gradient(G,L); E_c = sqrt(u.^2 + v.^2 + w.^2);
# E = max(max(max(E_c))); dims = [ww(1) ww(2) ww(3)];
# [rowsub colsub pagsub] = ind2sub(dims, find(E_c == E));
#
# Тут еще кусок с подпрограммой, который можно опустить
# %point of lightning initiation
# [row col pag] = sparkfinder(rowsub,colsub,pagsub);
# gf(1) = G(rowsub,colsub,pagsub); gf(2) = G(row,col,pag);
# [c,ii] = max(gf);
# i1 = rowsub; j1 = colsub; k1 = pagsub; i2 = row; j2 = col; k2 = pag;
# if ii == 2
#     i2 = rowsub; j2 = colsub; k2 = pagsub; i1 = row; j1 = col; k1 = pag;
# end
# Uu = abs(G(rowsub,colsub,pagsub) - G(row,col,pag));
# %+++++++++++++++++
# toc;
#
#
# Подпрограмма <function [ii jj kk] = sparkfinder(i,j,k)> ищет элемент с максимальным перепадом потенциала
# % spakfinder
# function [ii jj kk] = sparkfinder(i,j,k)
# global G ww
# a = 0;
# if i - 1 > 0
#     a = abs(G(i,j,k) - G(i-1,j,k));
#     ii = i-1; jj = j; kk = k;
# end
# if j - 1 > 0
#     if abs(G(i,j,k) - G(i,j-1,k)) > a
#         a = abs(G(i,j,k) - G(i,j-1,k));
#         ii = i; jj = j-1; kk = k;
#     end
# end
# if k - 1 > 0
#     if abs(G(i,j,k) - G(i,j,k-1)) > a
#         a = abs(G(i,j,k) - G(i,j,k-1));
#         ii = i; jj = j; kk = k-1;
#     end
# end
# if i + 1 < ww(1)
#     if abs(G(i,j,k) - G(i+1,j,k)) > a
#         a = abs(G(i,j,k) - G(i+1,j,k));
#         ii = i+1; jj = j; kk = k;
#     end
# end
# if j + 1 < ww(2)
#     if abs(G(i,j,k) - G(i,j+1,k)) > a
#         a = abs(G(i,j,k) - G(i,j+1,k));
#         ii = i; jj = j+1; kk = k;
#     end
# end
# if k + 1 < ww(3)
#     if abs(G(i,j,k) - G(i,j,k+1)) > a
#         a = abs(G(i,j,k) - G(i,j,k+1));
#         ii = i; jj = j; kk = k+1;
#     end
# end
# % ========== the end of the file ==============
