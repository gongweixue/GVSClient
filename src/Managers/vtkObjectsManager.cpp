#include "vtkObjectsManager.h"
#include <QMessageBox>
#include <QTime>
#include "Utils/GVSUtils.h"
#include "Utils/vtkTotallyInclude.h"


ObjectManager::ObjectManager(void)
{
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for (; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        model_iter->vecOfGeoObjs.clear();
    }
    treeOfGeoObjs.clear();

    this->pProgressDlg = new QProgressDialog();
}

ObjectManager::~ObjectManager(void)
{
    DelAllRdrsInObjTree();
    vector<Model>::iterator iter = treeOfGeoObjs.begin();
    for (; iter != treeOfGeoObjs.end(); ++iter)
    {
        iter->vecOfGeoObjs.clear();
    }
    treeOfGeoObjs.clear();

    if (pProgressDlg)
    {
        delete pProgressDlg;
    }
}

void ObjectManager::LoadDataForReadersInTree()
{
    ////progress to show the process of loading objects.
<<<<<<< HEAD
    QProgressDialog progressDlg;
    progressDlg.setWindowModality(Qt::ApplicationModal);
    progressDlg.setMinimumDuration(5);
    progressDlg.setWindowTitle(tr("请稍候"));
    progressDlg.setLabelText(QString());
    progressDlg.setCancelButtonText(tr("取消"));
    progressDlg.setRange(0,getNumOfObjsInTree());
    progressDlg.setCancelButton(0);
=======
    
    pProgressDlg->setWindowModality(Qt::ApplicationModal);
    pProgressDlg->setMinimumDuration(5);
    pProgressDlg->setWindowTitle(tr("请稍候"));
    pProgressDlg->setLabelText(QString());
    pProgressDlg->setCancelButtonText(tr("取消"));
    pProgressDlg->setRange(0,getNumOfObjsInTree());
    pProgressDlg->setCancelButton(0);
>>>>>>> GVSClient-23

    //update every record's reader.
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_iter != model_iter->vecOfGeoObjs.end(); obj_iter++)
        {
            QString loadingTip("正在加载模型：                \n");
            loadingTip.append(model_iter->modelName + "/" + obj_iter->getName().c_str());
<<<<<<< HEAD
            progressDlg.setLabelText(loadingTip);
=======
            pProgressDlg->setLabelText(loadingTip);

>>>>>>> GVSClient-23
            obj_iter->reader->Update();
            pProgressDlg->setValue(pProgressDlg->value() + 1);
            /*ReaderUpdater updater(this, obj_iter->reader);
            updater.start();
            updater.wait();*/
        }
    }

    pProgressDlg->hide();
    computeObjTreeBound();
}

void ObjectManager::DelAllRdrsInObjTree()
{
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_iter != model_iter->vecOfGeoObjs.end(); obj_iter++)
        {
            DeleteVTKPointer(obj_iter->reader);
        }
        model_iter->vecOfGeoObjs.clear();
    }
    computeObjTreeBound();
}

void ObjectManager::ClearObjTree()
{
    DelAllRdrsInObjTree();
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        model_iter->vecOfGeoObjs.clear();
    }
    treeOfGeoObjs.clear();
}

double* ObjectManager::computeObjTreeBound()
{
    for (int i = 0; i < 6; ++i)
    {
        m_bounds[i] = .0;
    }
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        vector<GeoObject>::iterator obj_iter = model_iter->vecOfGeoObjs.begin();
        for ( ; obj_iter != model_iter->vecOfGeoObjs.end(); obj_iter++)
        {
            double tmpBnds[6];
            obj_iter->reader->GetOutput()->GetBounds(tmpBnds);

            m_bounds[0] = (tmpBnds[0] < m_bounds[0])? tmpBnds[0] : m_bounds[0];
            m_bounds[1] = (tmpBnds[1] > m_bounds[1])? tmpBnds[1] : m_bounds[1];
            m_bounds[2] = (tmpBnds[2] < m_bounds[2])? tmpBnds[2] : m_bounds[2];
            m_bounds[3] = (tmpBnds[3] > m_bounds[3])? tmpBnds[3] : m_bounds[3];
            m_bounds[4] = (tmpBnds[4] < m_bounds[4])? tmpBnds[4] : m_bounds[4];
            m_bounds[5] = (tmpBnds[5] > m_bounds[5])? tmpBnds[5] : m_bounds[5];
        }
    }

    return m_bounds;
}

int ObjectManager::getNumOfObjsInTree()
{
    int ret = 0;
    vector<Model>::iterator model_iter = treeOfGeoObjs.begin();
    for ( ; model_iter != treeOfGeoObjs.end(); ++model_iter)
    {
        ret += model_iter->vecOfGeoObjs.size();
    }

    return ret;
}

QMutex progressValueMutex;
void ObjectManager::OnObjUpdateFinished()
{
    progressValueMutex.lock();
    this->pProgressDlg->setValue(this->pProgressDlg->value() + 1);
    progressValueMutex.unlock();
}

void ReaderUpdater::run()
{
    m_reader->Update();
    emit UpdateFinished();
}

