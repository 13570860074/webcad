<script setup lang="ts">
import { reactive, onMounted } from 'vue';
import { EDialogTrigger, UiDialog } from '@/Ui/Dialog/UiDialog';
import { gcadGlobal } from '@/gcadGlobal';
import Column from './Column.vue';
import Row from './Row.vue';

let props = defineProps<{ data: UiDialog }>()


onMounted(() => {
  init();
});



/**
 * 页面参数
 */
const page: {

  /** 尺寸位置 */
  height: number,
  width: number,
  position: { x: number, y: number },
  bHeight: boolean,
  bWidth: boolean,

  /** 标题 */
  title: string,
  titleHeight: number,
  icon: string,

  /** 对话框设置 */
  isFrame: boolean,                                     // 是否有边框
  borderRadius: number,                                 // 圆角
  backgroundColor: { r: number, g: number, b: number }  // 背景颜色

  /** 拖动对话框 */
  bMoveDialog: boolean,
  displacement: { clientX: number, clientY: number },
} = reactive({

  /** 尺寸位置 */
  height: props.data.height(),
  width: props.data.width(),
  position: { x: props.data.position().x, y: props.data.position().y },
  bHeight: false,
  bWidth: false,

  /** 标题 */
  titleHeight: props.data.titleHeight(),
  title: props.data.title(),
  icon: props.data.icon(),

  /** 对话框设置 */
  isFrame: props.data.isFrame(),
  borderRadius: 6,
  backgroundColor: props.data.backgroundColor(),

  /** 拖动对话框 */
  bMoveDialog: false,
  displacement: { clientX: 0, clientY: 0 },
});

const borderWidth: number = 0;


/** 初始化 */
const init = () => {

  /** 获得对话框尺寸 */
  let element = document.getElementById(`DialogElement${props.data.id}`);
  if (element != null) {
    page.width = element.getBoundingClientRect().width;
    page.height = element.getBoundingClientRect().height;
    page.bWidth = false;
    page.bHeight = false;
    if (props.data.width() > 0) {
      page.bWidth = true;
    }
    if (props.data.height() > 0) {
      page.bHeight = true;
    }
  }

  /** 设置默认图标 */
  if (page.icon == '') {
    page.icon = './resources/image/G.svg'
  }
  /** 设置默认位置 */
  if (page.position.x === -1) {
    let w: number = gcadGlobal.uiLayoutData.workRight - gcadGlobal.uiLayoutData.workLeft;
    page.position.x = w / 2.0;
    page.position.x -= page.width / 2;
    props.data.setPosition(page.position.x, page.position.y);
  }
  if (page.position.y === -1) {
    let h: number = gcadGlobal.uiLayoutData.workBottom - gcadGlobal.uiLayoutData.workTop;
    page.position.y = h / 2.0;
    page.position.y -= page.height / 2;
    props.data.setPosition(page.position.x, page.position.y);
  }

  /** 监听事件 */
  document.addEventListener('keydown', onkeydown);
  document.addEventListener('mousemove', onmousemove);
  document.addEventListener('mouseup', onmouseup);
  document.addEventListener('touchmove', ontouchmove);
  document.addEventListener('touchend', ontouchend);
  window.addEventListener('touchmove', ontouchmove);
  window.addEventListener('touchend', ontouchend);
}
/** 析构函数 */
const destructor = () => {
  console.log('析构函数');
  document.removeEventListener('keydown', onkeydown);
  document.removeEventListener('mousemove', onmousemove);
  document.removeEventListener('mouseup', onmouseup);
  document.removeEventListener('touchstart', ontouchstart);
  document.removeEventListener('touchmove', ontouchmove);
  document.removeEventListener('touchend', ontouchend);
}
/** 按下关闭按钮 */
const onclickClose = () => {
  props.data.close();
};


/** 监听设置对话框事件 */
props.data.trigger.on(EDialogTrigger.kDestructor, () => {
  destructor();
});
props.data.trigger.on(EDialogTrigger.kSetSize, (size: { width: number, height: number }) => {
  page.width = size.width;
  page.height = size.height;
  if(size.width > 0) {
    page.bWidth = true;
  } else {
    page.bWidth = false;
  }
  if(size.height > 0) {
    page.bHeight = true;
  } else {
    page.bHeight = false;
  }
});
props.data.trigger.on(EDialogTrigger.kSetTitle, (title: string) => {
  page.title = title;
});
props.data.trigger.on(EDialogTrigger.kSetIcon, (icon: string) => {
  page.icon = icon;
});
props.data.trigger.on(EDialogTrigger.kSetBackgroundColor, (color: { r: number, g: number, b: number }) => {
  page.backgroundColor = color;
});
props.data.trigger.on(EDialogTrigger.kSetFrame, (isFrame: boolean) => {
  page.isFrame = isFrame;
});
props.data.trigger.on(EDialogTrigger.kSetTitleHeight, (height: number) => {
  page.titleHeight = height;
});
props.data.trigger.on(EDialogTrigger.kSetPosition, (position: { x: number, y: number }) => {
  page.position = position;
});


/** 监听光标按下标题事件 */
const onmousedown = (e: MouseEvent) => {
  page.bMoveDialog = true;
  page.displacement.clientX = e.clientX;
  page.displacement.clientY = e.clientY;
}
/** 监听光标弹起事件 */
const onmouseup = () => {
  page.bMoveDialog = false;
}
/** 监听光标移动事件 */
const onmousemove = (e: MouseEvent) => {
  if (page.bMoveDialog === true) {
    let pt: { x: number, y: number } = { x: props.data.position().x, y: props.data.position().y };
    pt.x += (e.clientX - page.displacement.clientX);
    pt.y += (e.clientY - page.displacement.clientY);
    props.data.setPosition(pt.x, pt.y);
    page.displacement = { clientX: e.clientX, clientY: e.clientY };
  }
}
/** 监听键盘按下事件 */
const onkeydown = (e: KeyboardEvent) => {
  if (e.keyCode == 27) {
    onclickClose();
  }
}
/** 监听手指触摸事件 */
const ontouchstart = (e: TouchEvent) => {
  page.bMoveDialog = true;
  page.displacement.clientX = e.touches[0].clientX;
  page.displacement.clientY = e.touches[0].clientY;
}
/** 监听手指弹起事件 */
const ontouchend = () => {
  page.bMoveDialog = false;
}
/** 监听手指移动事件 */
const ontouchmove = (e: TouchEvent) => {
  if (page.bMoveDialog === true) {
    let pt: { x: number, y: number } = { x: props.data.position().x, y: props.data.position().y };
    pt.x += (e.touches[0].clientX - page.displacement.clientX);
    pt.y += (e.touches[0].clientY - page.displacement.clientY);
    props.data.setPosition(pt.x, pt.y);
    page.displacement = { clientX: e.touches[0].clientX, clientY: e.touches[0].clientY };
  }
}


</script>



<template>

  <div :id="`DialogElement${props.data.id}`" :style="{
    position: 'absolute', width: page.bWidth ? `${page.width}px` : '', height: page.bHeight ? `${page.height}px` : '',
    display: 'flex', left: `${page.position.x}px`, top: `${page.position.y}px`,
    borderRadius: `${page.borderRadius}px`, flexDirection: 'column',
    backgroundColor: `rgb(${page.backgroundColor.r},${page.backgroundColor.g},${page.backgroundColor.b})`,
  }" v-on:mouseup="onmouseup" v-on:mousemove="onmousemove" v-on:keydown="onkeydown">
    <!-- 标题栏 -->
    <Row>
      <div v-if="page.isFrame" :style="{
        width: `100%`, height: `${page.titleHeight}px`, backgroundColor: 'rgb(239,244,249)',
        borderRadius: `${page.borderRadius}px ${page.borderRadius}px ${page.borderRadius}px ${page.borderRadius}px`
      }" v-on:mousedown="($ele) => { onmousedown($ele) }" v-on:touchstart="($ele) => { ontouchstart($ele) }">

        <!-- 图标 -->
        <img :src="page.icon" :style="{
          width: `${page.titleHeight - 12}px`, height: `${page.titleHeight - 12}px`, margin: '6px', float: 'left',
        }"></img>

        <!-- 标题 -->
        <div :style="{
          userSelect: 'none', height: `${page.titleHeight - 12}px`, marginTop: '6px', marginBottom: '6px',
          float: 'left', fontSize: `${13}px`, color: 'rgb(0,0,0)',
          whiteSpace: 'pre', display: 'flex', alignItems: 'center'
        }">
          {{ page.title }}
        </div>

        <!-- 关闭按钮 -->
        <div :style="{
          width: `${page.titleHeight}px`, height: `${page.titleHeight}px`, float: 'right', fontSize: `${18}px`,
          backgroundColor: 'transparent', color: 'rgb(0,0,0)', borderRadius: `0px ${page.borderRadius}px 0px 0px`,
          whiteSpace: 'pre', display: 'flex', alignItems: 'center', justifyContent: 'center'
        }" v-on:mouseenter="($ele) => {
          (<HTMLElement>$ele.target).style.color = 'rgb(255,255,255)';
          (<HTMLElement>$ele.target).style.backgroundColor = 'rgb(255,0,0)';
        }" v-on:mouseout="($ele) => {
          (<HTMLElement>$ele.target).style.color = 'rgb(0,0,0)';
          (<HTMLElement>$ele.target).style.backgroundColor = 'transparent';
        }" v-on:click="onclickClose">×</div>

      </div>
    </Row>
    <!-- 内容栏 -->
    <Row>
      <Column :span="borderWidth"></Column>
      <Column :span="100 - borderWidth">
        <slot></slot>
      </Column>
      <Column :span="borderWidth"></Column>
    </Row>
  </div>

</template>
