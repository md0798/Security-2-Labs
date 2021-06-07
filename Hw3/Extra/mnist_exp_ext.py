from absl import app, flags
from easydict import EasyDict
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision
from datasets import MNISTDataset
import matplotlib.pyplot as plt
from cleverhans.torch.attacks.fast_gradient_method import fast_gradient_method
from cleverhans.torch.attacks.projected_gradient_descent import (
    projected_gradient_descent,
)
class CNN(torch.nn.Module):
    """Basic CNN architecture."""

    def __init__(self, in_channels=1):
        super(CNN, self).__init__()
        self.conv1 = nn.Conv2d(
            in_channels, 64, 8, 1
        )  # (batch_size, 3, 28, 28) --> (batch_size, 64, 21, 21)
        self.conv2 = nn.Conv2d(
            64, 128, 6, 2
        )  # (batch_size, 64, 21, 21) --> (batch_size, 128, 8, 8)
        self.conv3 = nn.Conv2d(
            128, 128, 5, 1
        )  # (batch_size, 128, 8, 8) --> (batch_size, 128, 4, 4)
        self.fc1 = nn.Linear(
            128 * 4 * 4, 128
        )  # (batch_size, 128, 4, 4) --> (batch_size, 2048)
        self.fc2 = nn.Linear(128, 10)  # (batch_size, 128) --> (batch_size, 10)

    def forward(self, x):
        x = F.relu(self.conv1(x))
        x = F.relu(self.conv2(x))
        x = F.relu(self.conv3(x))
        x = x.view(-1, 128 * 4 * 4)
        x = self.fc1(x)
        x = self.fc2(x)
        return x

def ld_mnist():
    """Load training and test data."""
    train_transforms = torchvision.transforms.Compose(
        [torchvision.transforms.ToTensor()]
    )
    test_transforms = torchvision.transforms.Compose(
        [torchvision.transforms.ToTensor()]
    )

    # Load MNIST dataset
    train_dataset = MNISTDataset(root="/tmp/data", transform=train_transforms)
    test_dataset = MNISTDataset(
        root="/tmp/data", train=False, transform=test_transforms
    )

    train_loader = torch.utils.data.DataLoader(
        train_dataset, batch_size=128, shuffle=True, num_workers=2
    )
    test_loader = torch.utils.data.DataLoader(
        test_dataset, batch_size=128, shuffle=False, num_workers=2
    )
    return EasyDict(train=train_loader, test=test_loader)


def main():
    # Load training and test data
    data = ld_mnist()

    net = CNN(in_channels=1)

    device = "cuda" if torch.cuda.is_available() else "cpu"
    if device == "cuda":
        net = net.cuda()
    loss_fn = torch.nn.CrossEntropyLoss(reduction="mean")
    optimizer = torch.optim.Adam(net.parameters(), lr=1e-3)

    net.load_state_dict(torch.load("mnist.pt"))
    # Evaluate on clean and adversarial data
    net.eval()
    report = EasyDict(nb_test=0, correct=0, correct_fgm=0, correct_pgd=0)
    exam = enumerate(data.test)
    batch_idx,(exam_data,exam_tar) = next(exam)
    plt.imshow(exam_data[112][0],cmap='gray',interpolation='none')
    plt.savefig("Original.png")
    target = torch.ones(128,dtype=torch.int64)
    target_lbl = target*8
    exam_data_fgm = fast_gradient_method(net,exam_data,0.2,np.inf,y=target_lbl,targeted=True)
    _,y_pred_fgm = net(exam_data_fgm).max(
            1
        )
    exam_data_fgm = (np.clip(exam_data_fgm.detach().numpy(),0,1))*255
    plt.imshow(exam_data_fgm[112][0],cmap='gray',interpolation='none')
    plt.savefig("fgsm_0.2.png")
    exam_data_fgm = fast_gradient_method(net,exam_data,2,2,y=target_lbl,targeted=True)
    _,y_pred_fgm = net(exam_data_fgm).max(
            1
        )
    exam_data_fgm = (np.clip(exam_data_fgm.detach().numpy(),0,1))*255
    plt.imshow(exam_data_fgm[112][0],cmap='gray',interpolation='none')
    plt.savefig("fgsm_2.png")
    exam_data_pgd = projected_gradient_descent(net,exam_data,0.2,0.01, 40,np.inf,y=target_lbl,targeted = True)
    _,y_pred_pgd = net(exam_data_pgd).max(
            1
        )
    exam_data_pgd = (np.clip(exam_data_pgd.detach().numpy(),0,1))*255
    plt.imshow(exam_data_pgd[112][0],cmap='gray',interpolation='none')
    plt.savefig("pgd_0.2.png")
    exam_data_pgd = projected_gradient_descent(net,exam_data,2,0.01, 40,2,y=target_lbl,targeted = True)
    _,y_pred_pgd = net(exam_data_pgd).max(
            1
        )
    exam_data_pgd = (np.clip(exam_data_pgd.detach().numpy(),0,1))*255
    plt.imshow(exam_data_pgd[112][0],cmap='gray',interpolation='none')
    plt.savefig("pgd_2.png")
    '''
    for x, y in data.test:
        x, y = x.to(device), y.to(device)
        
        _, y_pred = net(x).max(1)  # model prediction on clean examples
        report.nb_test += y.size(0)
        report.correct += y_pred.eq(y).sum().item()
    print(
        "test acc on clean examples (%): {:.3f}".format(
            report.correct / report.nb_test * 100.0
        )
    )
    '''
if __name__ == "__main__":

    main()
