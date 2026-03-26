<script setup lang="ts">




</script>



<template>

  <div class="custom-scrollbar" :style="{ overflowX: 'hidden', overflowY: 'auto', whiteSpace: 'nowrap' }">
    <slot></slot>
  </div>

</template>


<style scoped>
/* 设置滚动条背景 */
.custom-scrollbar::-webkit-scrollbar {
  width: 12px;
  height: 12px;
  background-color: transparent;
}

/** 滚动条的箭头 */
.custom-scrollbar::-webkit-scrollbar-button {
  width: 0;
  height: 0;
  display: none;
}

/** 纵向和横向滚动条重合的拐角处 */
.custom-scrollbar::-webkit-scrollbar-corner {
  background-color: transparent;
}

/** 被用于拖动的滑块 */
.custom-scrollbar::-webkit-scrollbar-thumb {
  min-height: 12px;
  border: 3px solid transparent;
  background-clip: content-box;
  border-radius: 6px;
  background-color: rgb(43, 51, 63);
}

/** 光标放到滑块里时的滑块 */
.custom-scrollbar::-webkit-scrollbar-thumb:hover {
  background-color: rgb(112, 112, 112);
}

/** 滚动时的滑块 */
.custom-scrollbar::-webkit-scrollbar-thumb:active {
  background-color: #87a2bd;
}

/** 滚动条下面的滚动槽 */
.custom-scrollbar::-webkit-scrollbar-track {
  background-color: transparent;
}

.custom-scrollbar::-webkit-scrollbar-track-piece {
  background-color: transparent;
}
</style>