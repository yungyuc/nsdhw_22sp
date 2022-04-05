import numpy as np
import copy
import matplotlib.pyplot as plt
import requests, gzip, os, hashlib

from collections import Counter

class FNN():

    class Linear:

        def __init__(self, input_size, output_size, activation) -> None:
            self._weight = np.random.uniform(-1., 1., size = (input_size, output_size)) / np.sqrt(input_size * output_size).astype(np.double)
            
            self._out = 0

            if activation == 'sigmoid':
                self._activate = FNN.Sigmoid()
            elif activation == 'softmax':
                self._activate = FNN.Softmax()
            elif activation == 'relu':
                self._activate = FNN.ReLu()
            elif activation == 'n':
                self._activate = FNN.Identical()
            else:
                raise RuntimeError("unknow activation function used")

        def forward(self, x):
            self._out = x.dot(self._weight)
            return self._activate.forward(self._out)

        def backward(self, x_input, grad_output):
            
            grad_output = grad_output * self._activate.df_dx(self._out)
            grad_input = np.dot(grad_output, self._weight.T)
            # grad_input = (self._weight.dot(grad_output.T)).T
            # _error = _error.dot(self._weight.T)
            
            grad_weights = np.dot(x_input.T, grad_output)

            self._out = 0
            # print(abs(grad_weights).sum())
            return grad_input, grad_weights

        def update(self, update, lr = 0.01):
            # print(update.sum())
            self._weight -= update * lr

        def __str__(self) -> str:
            return f"Linear Layer {self._weight.shape}"

    class Identical:

        def __init__(self) -> None:
            return

        def forward(self, x):
            return x
        
        def df_dx(self, x):
            return 1

        def __str__(self) -> str:
            return "None"

    class Sigmoid:

        def __init__(self) -> None:
            return

        def forward(self, x):
            return 1/(np.exp(-x)+1)

        def df_dx(self, x):
            return (np.exp(-x))/((np.exp(-x)+1)**2)

        def __str__(self) -> str:
            return "Sigmoid"

    class Softmax:

        def __init__(self) -> None:
            return

        def forward(self, x):
            exponents=np.exp(x)
            return exponents/np.sum(exponents)

        def df_dx(self, x):
            exp_element=np.exp(x-x.max())
            return exp_element/np.sum(exp_element,axis=0)*(1-exp_element/np.sum(exp_element,axis=0))


        def __str__(self) -> str:
            return "Softmax"

    class ReLu:

        def __init__(self) -> None:
            return

        def forward(self, x):
            return np.maximum(0, x)
        
        def df_dx(self, x):
            return np.where(x>0, 1, 0)

        def __str__(self) -> str:
            return "ReLu"

    def __init__(self, layers_config) -> None:
        
        self.layers = []

        # build layers
        for layer in layers_config.split('\n'):
            layer_list = layer.strip().split(',')

            # detect empty layer command
            if len(layer_list) <=0:
                raise IndexError("empty layer")

            # add linear layer
            if layer_list[0] == 'linear':

                try:
                    input_size = int(layer_list[1])
                    output_size = int(layer_list[2])
                    activation = layer_list[3].strip()
                except:
                    raise RuntimeError('invalid argument for linear layer')

                self.layers.append(self.Linear(input_size=input_size, output_size=output_size, activation=activation))
            
            # # add sigmoid layer
            # elif layer_list[0] == 'sigmoid':
            #     self.layers.append(self.Sigmoid())

            # # add softmax layer
            # elif layer_list[0] == 'softmax':
            #     self.layers.append(self.Softmax())

        # gradients
        self.grads = None

        # trian flag (allow gradients or not)
        self._train = True

    def summary(self) -> None:
        
        print('_'*20)
        for layer in self.layers:
            print(layer)
            print('_'*20)

    def forward(self, x):

        _x = x.copy()

        if self._train:
            self.layer_inputs = []
            self.layer_inputs.append(_x.copy())

        # feedforward
        for layer in self.layers:
            _x = layer.forward(_x)
            
            # require gradients
            if self._train:
                self.layer_inputs.append(_x.copy())

        return _x

    def backward(self, out, targets):


        # loss = self.softmax_crossentropy_with_logits(out, targets)
        loss_grad = self.grad_softmax_crossentropy_with_logits(out, targets)
        # loss_grad = 2*(out-targets)/out.shape[0]

        for layer_index in reversed(range(len(self.layers))):
            loss_grad, update = self.layers[layer_index].backward(self.layer_inputs[layer_index], loss_grad)
            self.layers[layer_index].update(update)

    def train(self):
        self._train = True

    def softmax_crossentropy_with_logits(self,logits,reference_answers):
        # Compute crossentropy from logits[batch,n_classes] and ids of correct answers
        logits_for_answers = logits[np.arange(len(logits)),reference_answers]
        
        xentropy = - logits_for_answers + np.log(np.sum(np.exp(logits),axis=-1))
        
        return xentropy
    
    def grad_softmax_crossentropy_with_logits(self,logits,reference_answers):
        # Compute crossentropy gradient from logits[batch,n_classes] and ids of correct answers
        # ones_for_answers = np.zeros_like(logits)
        # ones_for_answers[np.arange(len(logits)),reference_answers] = 1
        
        softmax = np.exp(logits) / np.exp(logits).sum(axis=-1,keepdims=True)
        
        return (- reference_answers + softmax) / logits.shape[0]

if __name__ == '__main__':

    path = r'C:\Users\user\OneDrive\桌面\project_test\MNIST'
    def fetch(url):
        fp = os.path.join(path, hashlib.md5(url.encode('utf-8')).hexdigest())
        if os.path.isfile(fp):
            with open(fp, "rb") as f:
                data = f.read()
        
        else:
            with open(fp, "wb") as f:
                data = requests.get(url).content
                f.write(data)

        return np.frombuffer(gzip.decompress(data), dtype=np.uint8).copy()

    X = fetch("http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz")[0x10:].reshape((-1, 28, 28))
    Y = fetch("http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz")[8:]
    X_test = fetch("http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz")[0x10:].reshape((-1, 28*28))
    Y_test = fetch("http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz")[8:]

    #Validation split
    rand=np.arange(60000)
    np.random.shuffle(rand)
    train_no=rand[:50000]

    val_no=np.setdiff1d(rand,train_no)

    X_train,X_val=X[train_no,:,:],X[val_no,:,:]
    Y_train,Y_val=Y[train_no],Y[val_no]

    with open('config', 'r') as fin:
        layer_config = fin.read()

    model = FNN(layers_config=layer_config)
    model.summary()
    model.train()

    targets = np.apply_along_axis(lambda a: np.array([0 if i != a else 1 for i in range(10)]), 1, Y_train.reshape((Y_train.shape[0], 1)))

    accuracies = []
    for i in range(100):
        # y = model.forward(np.zeros(shape=(100, 28*28)))
        y = model.forward(X_train.reshape(50000, 28*28))

        # model.backward(np.ones((100, 10)), np.random.random((100, 10)))
        model.backward(y, targets)

        category=np.argmax(y,axis=1)
        print(Counter(category))

        accuracy=(category==Y_train).mean()
        print(accuracy)
        accuracies.append(accuracy)

    plt.plot(accuracies)
    plt.show()
