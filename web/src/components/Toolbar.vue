<script setup lang="ts">
import { reactive } from 'vue';
import { gcadGlobal_uiLayoutData } from '@/gcadGlobal';
import { application } from '@/Application/FApplication';
import Row from './Row.vue';
import Column from './Column.vue';
import LayerComboBox from './LayerComboBox.vue';
import AttributeComboBox from './AttributeComboBox.vue';


/**
 * 页面参数
 */
const page: {
  top: number,
  left: number,
  tools: Array<{ name: string, icon: string, onclick?: () => void }>,
  tools2: Array<{ name: string, icon: string }>,
} = reactive({
  tools: [],
  tools2: [],
  top: 0,
  left: 0,
});
const workWidth: number = gcadGlobal_uiLayoutData().workRight - gcadGlobal_uiLayoutData().workLeft;
const width: number = (gcadGlobal_uiLayoutData().workRight - gcadGlobal_uiLayoutData().workLeft) * 0.7;
const menubarHeight: number = gcadGlobal_uiLayoutData().menubarHeight;
const toolbarHeight: number = gcadGlobal_uiLayoutData().toolbarHeight;
const fixedToolbarHeight: number = (toolbarHeight - 20) * 0.66;
const floatToolbarHeight: number = (toolbarHeight - 20) * 0.34;
page.top = menubarHeight + 8;
page.left = (workWidth - width) / 2.0;


const onclickPen = () => {
  application.kernelManager().docManagerPtr().sendStringToExecute(null, 'test ');
}

page.tools.push({ name: '钢笔', icon: './resources/svg/pen.svg', onclick: onclickPen });
page.tools.push({ name: '橡皮檫', icon: './resources/svg/eraser.svg' });
page.tools.push({ name: '编辑', icon: './resources/svg/edit.svg' });
page.tools.push({ name: '标注', icon: './resources/svg/dimension.svg' });
page.tools.push({ name: '视图', icon: './resources/svg/view.svg' });

page.tools2.push({ name: '视图', icon: './resources/svg/checkMark.svg' });
page.tools2.push({ name: '错误', icon: './resources/svg/error.svg' });


const onclick = (tool: { name: string, icon: string, onclick?: () => void }) => {
  if (tool.onclick != null && tool.onclick != undefined) {
    tool.onclick();
  }
}

/** 点击浮动条 */
let bDownFloatToolbar: boolean = false;
let floatToolbarStart: { clientX: number, clientY: number } = { clientX: 0, clientY: 0 };
let floatToolbarEnd: { clientX: number, clientY: number } = { clientX: 0, clientY: 0 };
const ondownFloatToolbar = (e: MouseEvent) => {
  bDownFloatToolbar = true;
  floatToolbarStart = { clientX: e.clientX, clientY: e.clientY };
};
window.addEventListener('mousemove', (e: MouseEvent) => {
  if (bDownFloatToolbar) {
    floatToolbarEnd = { clientX: e.clientX, clientY: e.clientY };
    page.left += floatToolbarEnd.clientX - floatToolbarStart.clientX;
    page.top += floatToolbarEnd.clientY - floatToolbarStart.clientY;
    if (page.top < 0) {
      page.top = 0;
    }
    if (page.top > (gcadGlobal_uiLayoutData().workBottom - gcadGlobal_uiLayoutData().workTop) - toolbarHeight) {
      page.top = (gcadGlobal_uiLayoutData().workBottom - gcadGlobal_uiLayoutData().workTop) - toolbarHeight;
    }
    if (page.left < 0) {
      page.left = 0;
    }
    if (page.left > (gcadGlobal_uiLayoutData().workRight - gcadGlobal_uiLayoutData().workLeft) - width) {
      page.left = (gcadGlobal_uiLayoutData().workRight - gcadGlobal_uiLayoutData().workLeft) - width;
    }
    floatToolbarStart = { clientX: floatToolbarEnd.clientX, clientY: floatToolbarEnd.clientY };
  }
});
window.addEventListener('mouseup', (e: MouseEvent) => {
  if (bDownFloatToolbar === true) {
    bDownFloatToolbar = false;
  }
});

</script>

<template>
  <div :style="{ position: 'absolute', borderRadius: '' }"></div>
  <Row :style="{
    position: 'absolute', zIndex: '1', top: `${page.top}px`, left: `${page.left}px`,
    width: `${width}px`, height: `${toolbarHeight}px`, backgroundColor: 'rgb(255,255,255)',
    borderRadius: '10px', flexDirection: 'column', userSelect: 'none', border: '1px solid rgb(204,204,204)'
  }">
    <!-- 拖动条 -->
    <Row :style="{ height: '10px', justifyContent: 'center', alignItems: 'center' }"
      v-on:mousedown="ondownFloatToolbar">
      <div :style="{
        width: `${width * 0.1}px`, height: '4px',
        backgroundColor: 'rgb(204,204,204)', borderRadius: '2px'
      }">
      </div>
    </Row>
    <!-- 固定工具栏 -->
    <Row :style="{ height: `${fixedToolbarHeight}px` }">
      <!-- 间隔 -->
      <Column :style="{ width: '20px' }"></Column>
      <!-- 图层以及特性 -->
      <Column :style="{ width: '300px' }">
        <!-- 图层 -->
        <Row :style="{ height: `50%`, alignItems: 'center' }">
          <LayerComboBox></LayerComboBox>
        </Row>
        <!-- 特性 -->
        <Row :style="{ height: `50%`, alignItems: 'center' }">
          <AttributeComboBox></AttributeComboBox>
        </Row>
      </Column>
      <!-- 分隔条 -->
      <Column :style="{ width: '20px', justifyContent: 'center', alignItems: 'center' }">
        <div :style="{ width: '1px', height: '80%', backgroundColor: 'rgb(204,204,204)' }"></div>
      </Column>
      <!-- 工具 -->
      <Column :style="{
        width: `100%`, justifyContent: 'center', alignItems: 'center', flexDirection: ''
      }">
        <Column v-for="tool in page.tools" :key="tool.name" :style="{
          width: '60px', height: `${fixedToolbarHeight}px`, justifyContent: 'center', alignItems: 'center'
        }">
          <!-- 图标 -->
          <div :style="{
            width: '100%', height: `${fixedToolbarHeight * 0.7}px`, borderRadius: '5px',
            display: 'flex', justifyContent: 'center', alignItems: 'center'
          }" v-on:click="onclick(tool)">
            <div :style="{
              width: '90%', display: 'flex',
              justifyContent: 'center', alignItems: 'center'
            }">
              <img :src="tool.icon" :style="{
                height: `${fixedToolbarHeight * 0.7 * 0.8}px`,
              }"></img>
            </div>
            <div :style="{ width: '10%' }"></div>
          </div>
          <!-- 文字 -->
          <div :style="{
            width: '100%', height: `${fixedToolbarHeight * 0.3}px`, display: 'flex',
            justifyContent: 'center', alignItems: 'center'
          }">
            <div :style="{ fontSize: '60%' }">{{ tool.name }}</div>
            <div :style="{ fontSize: '90%' }">▾</div>
          </div>
        </Column>
      </Column>
      <!-- 分隔条 -->
      <Column :style="{ width: '20px', justifyContent: 'center', alignItems: 'center' }">
        <div :style="{ width: '1px', height: '80%', backgroundColor: 'rgb(155,155,155)' }"></div>
      </Column>
      <!-- 工具2 -->
      <Column :style="{
        width: `${page.tools2.length * 50}px`,
        justifyContent: 'center', alignItems: 'center', flexDirection: ''
      }">
        <Column v-for="tool in page.tools2" :key="tool.name" :style="{
          width: '60px', height: `${fixedToolbarHeight}px`, justifyContent: 'center', alignItems: 'center'
        }">
          <!-- 图标 -->
          <div :style="{
            width: '100%', height: `${fixedToolbarHeight}px`, borderRadius: '5px',
            display: 'flex', justifyContent: 'center', alignItems: 'center'
          }">
            <div :style="{
              width: '90%', display: 'flex',
              justifyContent: 'center', alignItems: 'center'
            }">
              <img :src="tool.icon" :style="{ height: `${fixedToolbarHeight * 0.7}px` }"></img>
            </div>
          </div>
        </Column>
      </Column>
      <!-- 间隔 -->
      <Column :style="{ width: '10px' }"></Column>
    </Row>
    <!-- 分隔线 -->
    <Row :style="{ height: '5px', justifyContent: 'center', alignItems: 'center' }">
      <div :style="{ width: `${(width - 40)}px`, height: '1px', backgroundColor: 'rgb(204,204,204)' }"></div>
    </Row>
    <!-- 浮动工具栏 -->
    <Row :style="{ height: `${floatToolbarHeight}px` }"></Row>
    <!-- 底部空白 -->
    <Row :style="{ height: '5px' }"></Row>
  </Row>

</template>
