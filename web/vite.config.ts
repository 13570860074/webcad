import { defineConfig } from 'vite'
import { resolve } from 'path'
import vue from '@vitejs/plugin-vue'


// https://vitejs.dev/config/
export default defineConfig({
  plugins: [vue()],
  base: './',
  /* 路径配置 */
  resolve: { 
    alias: [
      {
        find: '@',
        replacement: resolve(__dirname, './src'),
      }
    ]
  },
  /* 开发服务器配置：WASM pthreads 依赖 SharedArrayBuffer，需要 COOP/COEP 响应头 */
  server: {
    headers: {
      'Cross-Origin-Opener-Policy': 'same-origin',
      'Cross-Origin-Embedder-Policy': 'require-corp',
    },
  },
  preview: {
    headers: {
      'Cross-Origin-Opener-Policy': 'same-origin',
      'Cross-Origin-Embedder-Policy': 'require-corp',
    },
  },
  /* 构建优化 */
  build: {
    rollupOptions: {
      output: {
        manualChunks: {
          'vendor-vue': ['vue'],
          'vendor-three': ['three'],
        },
      },
    },
  },
})
