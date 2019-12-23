Page({
  data: {

    key5: 'clock_sum',
    clock_number_key: [{
      key: 'clock1',
    },
    {
      key: 'clock2'
    },
    {
      key: 'clock3'
    },
    {
      key: 'clock4'
    },
    {
      key: 'clock5'
    },
    {
      key: 'clock6'
    },
    {
      key: 'clock7'
    }
    ],
    radioItems: [{
      name: '上午',
      value: 'am'
    },
    {
      name: '下午',
      value: 'pm'
    }
    ],

    am_or_pm_data: '',
    time_data: '',
    kind_data: '',
    number_data: '',

    temporary_clock_sum: 0,


    clock: [{
      am_or_pm: '',
      time: '',
      kind: '',
      number: '',
    }]
  },
  radioChange_am_or_pm: function (e) {
    if (e.detail.value.length == 1) {
      this.setData({
        am_or_pm_data: '0' + e.detail.value
      });
    } else {
      this.setData({
        am_or_pm_data: e.detail.value
      });
    }
    console.log("输入的上下午为：" + this.data.am_or_pm_data)
  },
  timeinput: function (e) {
    if (e.detail.value.length == 1) {
      this.setData({
        time_data: '0' + e.detail.value
      });
    } else {
      this.setData({
        time_data: e.detail.value
      });
    }
    console.log("输入的吃药时间为：" + this.data.time_data + "点")
  },
  kindinput: function (e) {
    if (e.detail.value.length == 1) {
      this.setData({
        kind_data: '0' + e.detail.value
      });
    } else {
      this.setData({
        kind_data: e.detail.value
      });
    }
    console.log("输入的小药盒所在位置为：" + this.data.kind_data)
  },
  numberinput: function (e) {
    if (e.detail.value.length == 1) {
      this.setData({
        number_data: '0' + e.detail.value
      });
    } else {
      this.setData({
        number_data: e.detail.value
      });
    }
    console.log("输入的吃药量为：" + this.data.number_data)
  },

  setStorageSync: function () {
    if (this.data.am_or_pm_data == 0 || this.data.time_data == 0 || this.data.kind_data == 0 || this.data.number_data == 0) {
      wx.showToast({
        title: '输入内容不能为空',
        icon: 'none'
      })
    } else if (this.data.time_data.length > 2 || this.data.kind_data.length > 2 || this.data.number_data.length > 2) {
      wx.showToast({
        title: '输入内容不符合格式',
        icon: 'none'
      })
    } else {
      wx.showToast({
        title: '保存成功',
        icon: 'none'
      })
      var obj = {
        am_or_pm: this.data.am_or_pm_data,
        time: this.data.time_data,
        kind: this.data.kind_data,
        number: this.data.number_data,
      }
      this.data.clock = this.data.clock.concat(obj); //记录新建闹钟的信息

      let key5 = this.data.key5;
      var temporary_clock_sum = wx.getStorageSync(key5);
      this.setData({
        temporary_clock_sum: temporary_clock_sum + 1
      });
      console.log("目前闹钟数：" + this.data.temporary_clock_sum);
      wx.setStorageSync(key5, this.data.temporary_clock_sum); //记录创立闹钟的个数

      let clock_number_key = this.data.clock_number_key;
      var clock = this.data.clock;
      clock.splice(0, 1);
      console.log("当次新建闹钟信息：" + this.data.clock[0].am_or_pm + " " + this.data.clock[0].time + " " + this.data.clock[0].kind + " " + this.data.clock[0].number);
      wx.setStorageSync(clock_number_key[this.data.temporary_clock_sum - 1].key, this.data.clock); //把当次的clock存进数据库

      wx.redirectTo({
        url: '../first page/first page'
      })
    }
  }
})