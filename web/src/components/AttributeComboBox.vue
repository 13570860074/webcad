<script setup lang="ts">
import { reactive, onMounted, onBeforeUnmount } from 'vue';
import Row from './Row.vue';
import Arx from '@/kernel/arxHeaders';
import Db from '@/kernel/Db';
import Cm from '@/kernel/Cm';
import Ap from '@/kernel/Ap';
import Scrollbar from './Scrollbar.vue';
import { gcadGlobal_uiLayoutData } from '@/gcadGlobal';
import { application } from '@/Application/FApplication';
import { EventState } from '@/EventManager/FEventManager';
import { UiSelectColorDialog } from '@/Ui/Dialog/UiSelectColorDialog';
import ModelBox from './ModelBox.vue';
import ColorComboBox from './ColorComboBox.vue';



onMounted(() => {
  let element = document.getElementById(attributeComboBoxElementId);
  if (element != null) {
    page.width = element.getBoundingClientRect().width;
    page.height = element.getBoundingClientRect().height;
  }
  init();
});
onBeforeUnmount(() => {

});




/**
 * 页面参数
 */
const page: {
  width: number,
  height: number,

  bExpand: boolean,
  layers: Array<{
    name: string,
    isLocked: boolean,
    isFrozen: boolean,
    isOff: boolean,
    bCurrent: boolean,
    bHighlight: boolean,
    colorIndex: number,
    color: { r: number, g: number, b: number }
  }>,

  box: {
    left: number,
    top: number,
    right: number,
    bottom: number,
  },
  menubarBox: {
    left: number,
    top: number,
    right: number,
    bottom: number,
  },
} = reactive({
  width: 24,
  height: 24,

  bExpand: false,
  layers: [],

  box: {
    left: 0,
    top: 0,
    right: 0,
    bottom: 0,
  },
  menubarBox: {
    left: 0,
    top: 0,
    right: 0,
    bottom: 0,
  },
});
const attributeComboBoxElementId: string = 'AttributeComboBoxElement';


/** 初始化 */
const init = () => {

}
/** 点击展开 */
const onclickExpand = () => {

  /** 获得图层对象 */
  let database: Db.DbDatabase = Arx.acdbHostApplicationServices().workingDatabase();
  if (database == null) {
    return;
  }
  let layerTable: Db.DbLayerTable = database.getLayerTable();
  if (layerTable == null) {
    return;
  }
  let layerTableIterator: Db.DbLayerTableIterator = layerTable.newIterator();
  layerTable.close();

  /** 获得当前图层 */
  let clayerId: Db.DbObjectId = database.clayer();

  /** 获得所有图层对象 */
  page.layers = [];
  for (layerTableIterator.start(); !layerTableIterator.done(); layerTableIterator.step()) {
    let layerTableRecord: Db.DbLayerTableRecord = layerTableIterator.getRecord();
    if (layerTableRecord == null) {
      continue;
    }

    let name: string = layerTableRecord.getName();
    let isLocked: boolean = layerTableRecord.isLocked();
    let isFrozen: boolean = layerTableRecord.isFrozen();
    let isOff: boolean = layerTableRecord.isOff();
    let bCurrent: boolean = false;
    if (clayerId.asOldId() === layerTableRecord.objectId().asOldId()) {
      bCurrent = true;
    }
    let color: Cm.CmColor = layerTableRecord.color();
    let rgb: { r: number, g: number, b: number } = { r: color.red(), g: color.green(), b: color.blue() };
    page.layers.push({
      name: name,
      isLocked: isLocked,
      isFrozen: isFrozen,
      isOff: isOff,
      color: rgb,
      bCurrent: bCurrent,
      bHighlight: false,
      colorIndex: color.colorIndex(),
    });
    layerTableRecord.close();
  }
  Arx.emsdk_free(layerTableIterator);

  /** 展开 */
  page.bExpand = true;
  let element: HTMLElement = document.getElementById('gcad');
  let layerComboBoxElement: HTMLElement = document.getElementById(attributeComboBoxElementId);
  if (element != null && layerComboBoxElement != null) {
    let rect: DOMRect = element.getBoundingClientRect();
    page.box.left = rect.left;
    page.box.top = rect.top;
    page.box.right = rect.right;
    page.box.bottom = rect.bottom;

    page.menubarBox.left = layerComboBoxElement.getBoundingClientRect().left - rect.left;
    page.menubarBox.top = layerComboBoxElement.getBoundingClientRect().bottom - rect.top;
    page.menubarBox.right = layerComboBoxElement.getBoundingClientRect().right - rect.left;
  }
}
/** 点击关闭展开 */
const onclickCloseExpand = () => {
  page.bExpand = false;
}
/** 点击颜色 */
const onclickColor = async (layer: { name: string, colorIndex: number, isLocked: boolean, isFrozen: boolean, isOff: boolean }) => {
  let dialog: UiSelectColorDialog = new UiSelectColorDialog();
  dialog.setTitle('选择颜色');
  dialog.setColorIndex(layer.colorIndex);
  dialog.onConfirm((colorIndex: number) => {
    if (colorIndex <= 0 || colorIndex > 255) {
      return;
    }
    let database: Db.DbDatabase = Arx.acdbHostApplicationServices().workingDatabase();
    if (database == null) {
      return;
    }
    let layerTable: Db.DbLayerTable = database.getLayerTable();
    if (layerTable == null) {
      return;
    }
    let layerTableRecord: Db.DbLayerTableRecord = layerTable.getAt(layer.name, Db.OpenMode.kForWrite);
    if (layerTableRecord == null) {
      return;
    }
    let color: Cm.CmColor = new Cm.CmColor();
    color.setColorIndex(colorIndex);
    layerTableRecord.setColor(color);
    layerTableRecord.close();
    layerTable.close();
    Arx.emsdk_free(color);
  });
  dialog.exec();
}
/** 鼠标移入菜单栏 */
const onmouseenter = (item: { bHighlight: boolean }) => {
  for (let i = 0; i < page.layers.length; i++) {
    page.layers[i].bHighlight = false;
  }
  item.bHighlight = true;
}


</script>

<template>
  <Row :style="{ height: '100%', alignItems: 'center' }">

    <!-- 特性图标 -->
    <img :src="'./resources/svg/featureMatching.svg'"
      :style="{ width: `${page.height}px`, height: `${page.height}px` }"></img>

    <!-- 下拉条 -->
    <div :id="attributeComboBoxElementId" :style="{
      width: '100%', height: '100%', marginLeft: '5px',
      display: 'flex', justifyContent: 'center', alignItems: 'center'
    }" v-on:click="onclickExpand">
        <ColorComboBox></ColorComboBox>
    </div>

    <!-- 展开按钮 -->
    <img :src="'./resources/svg/expand.svg'" :style="{
      marginLeft: '5px', width: `${page.height}px`, height: `${page.height}px`
    }"></img>
  </Row>


</template>
