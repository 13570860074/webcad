<script setup lang="ts">
import { gcadGlobal } from '@/gcadGlobal';
import { reactive, onMounted, onBeforeUnmount } from 'vue';


onMounted(() => {
  let element = document.getElementById('gcad');
  if (element != null) {
    page.width = element.getBoundingClientRect().width;
    page.height = element.getBoundingClientRect().height;
    page.left = element.getBoundingClientRect().left;
    page.top = element.getBoundingClientRect().top;
  }
  init();
});
onBeforeUnmount(() => {
  gcadGlobal.globalZIndex--;
});

/**
 * 页面参数
 */
const page: {
  zIndex: number,
  width: number,
  height: number,
  left: number,
  top: number,
} = reactive({
  zIndex: 0,
  width: 24,
  height: 24,
  left: 0,
  top: 0,
});


const init = () => {
  page.zIndex = gcadGlobal.globalZIndex;
  gcadGlobal.globalZIndex++;
}

</script>

<template>
  <div :style="{
    zIndex: `${page.zIndex}`, position: 'absolute', width: `${page.width}px`, height: `${page.height}px`,
    left: `${page.left}px`, top: `${page.top}px`
  }">
    <slot></slot>
  </div>
</template>
