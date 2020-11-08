from numpy import exp

def ExpLawT (X, Mod, Exp, X0):
    return Mod*exp((X-X0)*Exp)
def PowLawT (X, Mod, Exp, X0):
    return Mod*((X-X0)**Exp)
def GammaLawT (X, Mod, Exp1, Exp2, X0):
    return Mod*((X-X0)**Exp1)*exp((X-X0)*Exp2)
def MaxBoltT (X, Mod, Exp1, Exp2, X0):
    return Mod*((X-X0)**Exp1)*exp(((X-X0)**Exp1)*Exp2)
def SelfExpT (X, Mod, Exp, X0):
    return (Mod*(X-X0))**(Exp*(X-X0))

def Line (X, A, B):
	return A*X+B
def ExpLaw (X, Mod, Exp):
    return Mod*exp(X*Exp)
def PowLaw (X, Mod, Exp):
    return Mod*(X**Exp)
def GammaLaw (X, Mod, Exp1, Exp2):
    return Mod*(X**Exp1)*exp(X*Exp2)