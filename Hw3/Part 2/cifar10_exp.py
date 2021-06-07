from absl import app, flags
from easydict import EasyDict
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision
import matplotlib.pyplot as plt
from cleverhans.torch.attacks.fast_gradient_method import fast_gradient_method
from cleverhans.torch.attacks.projected_gradient_descent import (
    projected_gradient_descent,
)

FLAGS = flags.FLAGS


class CNN(torch.nn.Module):
    """Basic CNN architecture."""

    def __init__(self, in_channels=1):
        super(CNN, self).__init__()
        self.conv1 = nn.Conv2d(in_channels, 64, 8, 1)
        self.conv2 = nn.Conv2d(64, 128, 6, 2)
        self.conv3 = nn.Conv2d(128, 128, 5, 2)
        self.fc = nn.Linear(128 * 3 * 3, 10)

    def forward(self, x):
        x = F.relu(self.conv1(x))
        x = F.relu(self.conv2(x))
        x = F.relu(self.conv3(x))
        x = x.view(-1, 128 * 3 * 3)
        x = self.fc(x)
        return x


def ld_cifar10():
    """Load training and test data."""
    train_transforms = torchvision.transforms.Compose(
        [torchvision.transforms.ToTensor()]
    )
    test_transforms = torchvision.transforms.Compose(
        [torchvision.transforms.ToTensor()]
    )
    train_dataset = torchvision.datasets.CIFAR10(
        root="/tmp/data", train=True, transform=train_transforms, download=True
    )
    test_dataset = torchvision.datasets.CIFAR10(
        root="/tmp/data", train=False, transform=test_transforms, download=True
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
    data = ld_cifar10()

    # Instantiate model, loss, and optimizer for training
    net = CNN(in_channels=3)
    device = "cuda" if torch.cuda.is_available() else "cpu"
    if device == "cuda":
        net = net.cuda()
    loss_fn = torch.nn.CrossEntropyLoss(reduction="mean")
    optimizer = torch.optim.Adam(net.parameters(), lr=1e-3)

    net.load_state_dict(torch.load("cifar10.pt", map_location=device))
    # Evaluate on clean and adversarial data
    net.eval()
    report = EasyDict(nb_test=0, correct=0, correct_fgm=0, correct_pgd=0,correct_fgm_2=0, correct_pgd_2=0)
    dataiter = iter(data.test)
    imgs,lblb = dataiter.next()
    img = torchvision.utils.make_grid(imgs[1])
    npimg = img.numpy()
    plt.imshow(np.transpose(npimg,(1,2,0)))
    plt.savefig("Original.png")
    img_fgm = fast_gradient_method(net, imgs, 0.2, np.inf)
    img_fgm = torchvision.utils.make_grid(img_fgm[1])
    npimg = np.clip(img_fgm.detach().numpy(),0,1) 
    plt.imshow(np.transpose(npimg,(1,2,0)))
    plt.savefig("fgsm_0.2.png")
    img_fgm = fast_gradient_method(net, imgs, 2, 2)
    img_fgm = torchvision.utils.make_grid(img_fgm[1])
    npimg = np.clip(img_fgm.detach().numpy(),0,1)
    plt.imshow(np.transpose(npimg,(1,2,0)))
    plt.savefig("fgsm_2.png")    
    img_pgd = projected_gradient_descent(net,imgs,0.2,0.01, 40,np.inf)
    img_pgd = torchvision.utils.make_grid(img_pgd[1])
    npimg = np.clip(img_pgd.detach().numpy(),0,1)
    plt.imshow(np.transpose(npimg,(1,2,0)))
    plt.savefig("pgd_0.2.png")
    img_pgd = projected_gradient_descent(net,imgs,2,0.01, 40,2)
    img_pgd = torchvision.utils.make_grid(img_pgd[1])
    npimg = np.clip(img_pgd.detach().numpy(),0,1)
    plt.imshow(np.transpose(npimg,(1,2,0)))
    plt.savefig("pgd_2.png")    
    '''
    for x, y in data.test:
        x, y = x.to(device), y.to(device)
        x_fgm = fast_gradient_method(net, x, 0.2, 2)
        x_pgd = projected_gradient_descent(net, x, 0.2, 0.01, 40, np.inf)
        x_fgm_2 = fast_gradient_method(net, x, 2, 2)
        x_pgd_2 = projected_gradient_descent(net, x, 2, 0.01, 40, np.inf)
        _, y_pred = net(x).max(1)  # model prediction on clean examples
        _, y_pred_fgm = net(x_fgm).max(
            1
        )  # model prediction on FGM adversarial examples
        _, y_pred_pgd = net(x_pgd).max(
            1
        )  # model prediction on PGD adversarial examples
        _, y_pred_fgm_2 = net(x_fgm_2).max(
            1
        )  # model prediction on FGM adversarial examples
        _, y_pred_pgd_2 = net(x_pgd_2).max(
            1
        )  # model prediction on PGD adversarial examples
        
        report.nb_test += y.size(0)
        report.correct += y_pred.eq(y).sum().item()
        report.correct_fgm += y_pred_fgm.eq(y).sum().item()
        report.correct_pgd += y_pred_pgd.eq(y).sum().item()
        report.correct_fgm_2 += y_pred_fgm_2.eq(y).sum().item()
        report.correct_pgd_2 += y_pred_pgd_2.eq(y).sum().item()
    print(
        "test acc on clean examples (%): {:.3f}".format(
            report.correct / report.nb_test * 100.0
        )
    )
    print(
        "test acc on FGM adversarial examples eps = 0.2 (%): {:.3f}".format(
            report.correct_fgm / report.nb_test * 100.0
        )
    )
    print(
        "test acc on PGD adversarial examples eps = 0.2 (%): {:.3f}".format(
            report.correct_pgd / report.nb_test * 100.0
        )
    )
    print(
        "test acc on FGM adversarial examples eps = 2 (%): {:.3f}".format(
            report.correct_fgm_2 / report.nb_test * 100.0
        )
    )
    print(
        "test acc on PGD adversarial examples eps = 2 (%): {:.3f}".format(
            report.correct_pgd_2 / report.nb_test * 100.0
        )
    )
    '''

if __name__ == "__main__":

    main()
